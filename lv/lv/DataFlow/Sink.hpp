// *********************************************************************
//  Sink   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_SINK_HPP
#define LV_DATAFLOW_SINK_HPP

#include <lv/Buffer.hpp>
#include <lv/MemFn.hpp>

#include <lv/DataFlow/Registery.hpp>

#include <memory>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>

namespace lv { namespace flow {

	typedef boost::function<void(slot_type, BufferPtr)> proxy_push_type;

	/// thread-safe
	template<template<class> class PushPolicy, class Key, class IArchive>
	class Sink : boost::noncopyable
	{
		detail::Registery<Key, IArchive> registery_;

		typedef PushPolicy<BufferPtr> push_policy_type;
		push_policy_type	push_policy_;

	public:

		/**
		 * @proxy_push you can handle exceptions using a proxy push
		 */
		Sink(proxy_push_type const & proxy_push = proxy_push_type(), push_policy_type const & policy = push_policy_type())
			: push_policy_(policy)
		{
			slot_type slot = boost::bind(&Sink::push_impl, this, _1);

			if(proxy_push)
			{
				push_policy_.set_callback(boost::bind(proxy_push, slot, _1));
			}
			else
			{
				push_policy_.set_callback(slot);
			}
		}

		void	push(BufferPtr buf)
		{
			push_policy_(buf);
		}

		/**
		 * register a member function with a given object
		 * @param t could be an object, a pointer, a reference wrapper or 
		 *	a smart pointer.
		 */
		template<class MemFn, class T>
		Sink & reg_mem_fn(Key const & key, MemFn f, T t)
		{
			return reg<typename BindMemFnSignature<MemFn>::type>(key, bind_mem_fn(f, t));
		}

		/**
		 * register a function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class F>
		Sink & reg(Key const & key, F f)
		{
			return reg<typename Signature<F>::type, F>(key, f);
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class Signature, class F>
		Sink & reg(Key const & key, F f)
		{
			registery_.reg<Signature>(key, f);
			return *this;
		}


		/// clear all the registered functions
		void clear()
		{
			registery_.clear();
		}

	private:

		void	push_impl(BufferPtr buf)
		{
			boost::iostreams::stream_buffer<boost::iostreams::array_source> raw_is(&(*buf)[0], buf->size());
			IArchive ia(raw_is);

			registery_.invoke(ia);
		}
	};

} }

#endif