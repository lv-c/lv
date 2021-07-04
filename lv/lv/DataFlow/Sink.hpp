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

#include <lv/DataFlow/Registery.hpp>
#include <lv/Serialization/IArchiveWrapper.hpp>
#include <lv/MPL/MemFn.hpp>

#include <boost/noncopyable.hpp>

#include <memory>


namespace lv::flow
{
	/// thread-safe
	template<class Key, class IArchive>
	class Sink : boost::noncopyable
	{
	public:

		using key_type = Key;
		using iarchive_type = IArchive;

	private:

		detail::Registery<key_type, iarchive_type> registery_;

		IStreamFactory	istream_factory_;

	public:

		void	push(ConstBufferRef buf)
		{
			IArchiveWrapper<IArchive> ia(istream_factory_, buf);
			registery_.invoke(ia.get());
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

	};

}
