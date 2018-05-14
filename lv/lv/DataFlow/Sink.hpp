// *********************************************************************
//  Sink   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>
#include <lv/Serialization/IArchiveWrapper.hpp>
#include <lv/MPL/MemFn.hpp>
#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Registery.hpp>

#include <boost/noncopyable.hpp>

#include <memory>
#include <functional>


namespace lv::flow
{
	using proxy_push_type = std::function<void(slot_type const &, ConstBufferRef const &)>;

	/// thread-safe
	template<template<class> class PushPolicy, class Key, class IArchive>
	class Sink : boost::noncopyable
	{
	public:

		using key_type = Key;
		using iarchive_type = IArchive;

		using push_policy_type = PushPolicy<ConstBufferRef>;

	private:

		detail::Registery<key_type, iarchive_type> registery_;

		push_policy_type	push_policy_;

		using IStreamFactoryPtr = std::shared_ptr<IStreamFactory>;
		using WeakIStreamFactoryPtr = std::weak_ptr<IStreamFactory>;

		IStreamFactoryPtr	istream_factory_;

	public:

		/**
		 * @proxy_push you can handle exceptions using a proxy push
		 */
		Sink(proxy_push_type proxy_push = proxy_push_type(), push_policy_type policy = push_policy_type())
			: push_policy_(std::move(policy))
			, istream_factory_(std::make_shared<IStreamFactory>())
		{
			slot_type slot = [this](ConstBufferRef const & buf) { push_impl(buf, istream_factory_); };

			if (proxy_push)
			{
				push_policy_.set_callback([proxy_push = std::move(proxy_push), slot](ConstBufferRef const & buf) {
					proxy_push(slot, buf);
				});
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
		Sink &	reg_mem_fn(key_type const & key, MemFn f, T && t)
		{
			return reg<typename BindMemFnSignature<MemFn>::type>(key, bind_mem_fn(f, std::forward<T>(t)));
		}

		/**
		 * register a function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class F>
		Sink &	reg(key_type const & key, F && f)
		{
			return reg<typename Signature<F>::type, F>(key, std::forward<F>(f));
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class Signature, class F>
		Sink &	reg(key_type const & key, F && f)
		{
			registery_.template reg<Signature>(key, std::forward<F>(f));
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
				IArchiveWrapper<IArchive> ia(*factory, buf);
				registery_.invoke(ia.get());
			}
		}
	};

}
