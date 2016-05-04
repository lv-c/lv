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

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>

#include <map>
#include <shared_mutex>


namespace lv { namespace flow {

	class SerializationError : public boost::archive::archive_exception
	{
		typedef boost::archive::archive_exception	base_type;

		std::string const msg_;

	public:

		SerializationError(boost::archive::archive_exception::exception_code code, std::string const & msg)
			: boost::archive::archive_exception(code)
			, msg_(base_type::what() + msg)
		{
		}

		virtual char const * what() const noexcept
		{
			return msg_.c_str();
		}
	};

	namespace detail
	{
		template<class Key, class IArchive>
		class Registery : boost::noncopyable
		{

			typedef std::function<void(IArchive &)>	invoker_type;

			typedef std::map<Key, invoker_type>	invoker_map;

			invoker_map		invokers_;

			std::shared_timed_mutex	mutex_;

		public:

			/**
			 * @exception std::runtime_error if @a key has already been used
			 */
			template<class Signature, class F>
			void	reg(Key const & key, F f)
			{
				std::lock_guard<std::shared_timed_mutex> lock(mutex_);

				if (invokers_.find(key) != invokers_.end())
				{
					throw std::runtime_error(std::string("The key has already been used:") + 
						boost::lexical_cast<std::string>(key));
				}

				invokers_.insert(std::make_pair(key, detail::Invoker<Signature, IArchive>(f)));
			}


			/**
			 * @exception SerializationError  ( public boost::archive::archive_exception)
			 * @exception runtime_error exceptions thrown by the target function
			 */
			void	invoke(IArchive & ia)
			{
				Key key;
				ia >> key;

				invoker_map::iterator it;

				{
					// no deletion, so the iterator will be valid
					std::shared_lock<std::shared_timed_mutex> lock(mutex_);

					it = invokers_.find(key);
					if (it == invokers_.end())
					{
						return;
					}
				}

				try
				{
					it->second(ia);
				}
				catch(boost::archive::archive_exception const & ex)
				{
					throw SerializationError(ex.code, std::string(".error calling function:") + boost::lexical_cast<std::string>(key));
				}
				catch(std::exception const & ex)
				{
					throw std::runtime_error(ex.what() + std::string(".error calling function:") + boost::lexical_cast<std::string>(key));
				}
			}

		};
	}

	

} }

#endif 