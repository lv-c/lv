// *********************************************************************
//  Registery   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_REGISTERY_HPP
#define LV_DATAFLOW_REGISTERY_HPP

#include <lv/Exception.hpp>
#include <lv/Functional.hpp>

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Invoker.hpp>


#include <string>
#include <map>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>

namespace lv { namespace flow {

	DEFINE_EXCEPTION_MSG(InvalidKey, std::runtime_error);

	class SerializationError : public boost::archive::archive_exception
	{
		typedef boost::archive::archive_exception	base_type;

		std::string const msg_;

	public:

		SerializationError(boost::archive::archive_exception::exception_code code, std::string const & msg)
			: boost::archive::archive_exception(code)
			, msg_(std::string(base_type::what()) + msg)
		{
		}

		virtual char const * what() const
		{
			return msg_.c_str();
		}
	};


	template<class Key, class IArchive>
	class Registery : boost::noncopyable
	{
		
		typedef boost::function<void(IArchive &)>	invoker_type;

		typedef std::map<Key, invoker_type>	invoker_map;

		invoker_map	invokers_;

		bool	reg_ended_;

	private:

		// to be called by the Sink class

		template<template<class> class, class, class> friend class Sink;


		/**
		 * @exception InvalidKey no function found associated with the given key
		 * @exception SerializationError  ( public boost::archive::archive_exception)
		 * @exception exceptions thrown by the target function
		 */
		void	invoke(IArchive & ia)
		{
			Key key;
			ia >> key;

			invoker_map::iterator it = invokers_.find(key);
			if(it == invokers_.end())
				throw InvalidKey(std::string("Invalid key:") + boost::lexical_cast<std::string>(key));

			try
			{
				it->second(ia);
			}
			catch (boost::archive::archive_exception const & ex)
			{
				throw SerializationError(ex.code, std::string(".error calling function:") + boost::lexical_cast<std::string>(key));
			}
			catch(...)
			{
				throw;
			}
		}

		void	end_reg()
		{
			reg_ended_ = true;
		}

	public:

		Registery()
			: reg_ended_(false)
		{
		}

		/**
		 * register a function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class F>
		Registery & reg(Key const & key, F f)
		{
			return reg<typename Signature<F>::type, F>(key, f);
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a key has already been used
		 */
		template<class Signature, class F>
		Registery & reg(Key const & key, F f)
		{
			if(reg_ended_)
			{
				BOOST_ASSERT(false && "You should not register functions any more");
				return *this;
			}

			if(invokers_.find(key) != invokers_.end())
			{
				throw std::runtime_error(std::string("The key has already been used:") + 
					boost::lexical_cast<std::string>(key));
			}

			invokers_.insert(std::make_pair(key, detail::Invoker<Signature, IArchive>(f)));

			return *this;
		}

	};

} }

#endif 