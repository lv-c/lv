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
#include <lv/Serialization/IArchiveCreator.hpp>

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Registery.hpp>

#include <boost/noncopyable.hpp>

#include <memory>
#include <functional>


namespace lv { namespace flow {

	typedef std::function<void(slot_type const &, ConstBufferRef const &)> proxy_push_type;

	/// thread-safe
	template<template<class> class PushPolicy, class Key, class IArchive>
	class Sink : boost::noncopyable
	{
	public:

		typedef typename Key		key_type;
		typedef typename IArchive	iarchive_type;

		typedef PushPolicy<ConstBufferRef> push_policy_type;

	private:

		detail::Registery<key_type, iarchive_type> registery_;

		push_policy_type	push_policy_;

		typedef std::shared_ptr<IStreamFactory>		IStreamFactoryPtr;
		typedef std::weak_ptr<IStreamFactory>		WeakIStreamFactoryPtr;

		IStreamFactoryPtr	istream_factory_;

	public:

		/**
		 * @proxy_push you can handle exceptions using a proxy push
		 */
		Sink(proxy_push_type const & proxy_push = proxy_push_type(), push_policy_type const & policy = push_policy_type())
			: push_policy_(policy)
			, istream_factory_(std::make_shared<IStreamFactory>())
		{
			slot_type slot = std::bind(&Sink::push_impl, this, std::placeholders::_1, WeakIStreamFactoryPtr(istream_factory_));

			if (proxy_push)
			{
				push_policy_.set_callback(std::bind(proxy_push, slot, std::placeholders::_1));
			}
			else
			{
				push_policy_.set_callback(slot);
			}
		}

		void	push(ConstBufferRef const & buf)
		{
			push_policy_(buf);
		}

		/**
		 * register a member function with a given object
		 * @param t could be an object, a pointer, a reference wrapper or 
		 *	a smart pointer.
		 */
		template<class MemFn, class T>
		Sink &	reg_mem_fn(key_type const & key, MemFn f, T t)
		{
			return reg<typename BindMemFnSignature<MemFn>::type>(key, bind_mem_fn(f, t));
		}

		/**
		 * register a function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class F>
		Sink &	reg(key_type const & key, F f)
		{
			return reg<typename Signature<F>::type, F>(key, f);
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class Signature, class F>
		Sink &	reg(key_type const & key, F f)
		{
			registery_.reg<Signature>(key, f);
			return *this;
		}

		void	stop()
		{
			istream_factory_.reset();
		}

	private:

		void	push_impl(ConstBufferRef const & buf, WeakIStreamFactoryPtr weak_isteram_factory)
		{
			if (IStreamFactoryPtr factory = weak_isteram_factory.lock())
			{
				IArchiveCreator<IArchive> creator(*factory, buf);
				registery_.invoke(creator.archive());
			}
		}
	};

} }

#endif