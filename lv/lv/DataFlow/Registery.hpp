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
#include <boost/thread/shared_mutex.hpp>

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

	namespace detail
	{
		template<class Key, class IArchive>
		class Registery : boost::noncopyable
		{

			typedef boost::function<void(IArchive &)>	invoker_type;

			typedef std::map<Key, invoker_type>	invoker_map;

			invoker_map	invokers_;

			boost::shared_mutex shared_mutex_;

		public:


			/**
			 * @exception std::runtime_error if @a key has already been used
			 */
			template<class Signature, class F>
			void reg(Key const & key, F f)
			{
				// a write lock
				boost::unique_lock<boost::shared_mutex> lock(shared_mutex_);

				if(invokers_.find(key) != invokers_.end())
				{
					throw std::runtime_error(std::string("The key has already been used:") + 
						boost::lexical_cast<std::string>(key));
				}

				invokers_.insert(std::make_pair(key, detail::Invoker<Signature, IArchive>(f)));
			}


			/**
			 * @exception SerializationError  ( public boost::archive::archive_exception)
			 * @exception exceptions thrown by the target function
			 */
			void	invoke(IArchive & ia)
			{
				Key key;
				ia >> key;

				// a read lock
				boost::shared_lock<boost::shared_mutex> lock(shared_mutex_);

				invoker_map::iterator it = invokers_.find(key);
				if(it == invokers_.end())
					return;

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


		};
	}

	

} }

#endif 