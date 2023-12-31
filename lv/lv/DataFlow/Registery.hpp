// *********************************************************************
//  Registery   version:  1.0   ��  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Invoker.hpp>

#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>
#include <lv/MPL/Functional.hpp>
#include <lv/Concurrent/SpinMutex.hpp>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>

#include <map>


namespace lv::flow
{
	class SerializationError : public boost::archive::archive_exception
	{
		using base_type = boost::archive::archive_exception;

		std::string const msg_;

	public:

		SerializationError(boost::archive::archive_exception::exception_code code, std::string const & msg)
			: boost::archive::archive_exception(code)
			, msg_(base_type::what() + msg)
		{
		}

		char const * what() const noexcept override
		{
			return msg_.c_str();
		}
	};

	namespace detail
	{
		template<class Key, class IArchive>
		class Registery : boost::noncopyable
		{

			using invoker_type = std::function<void(IArchive &)>;

			using invoker_map = std::map<Key, invoker_type>;

			invoker_map		invokers_;

			SpinMutex		mutex_;

		public:

			/**
			 * @exception std::runtime_error if @a key has already been used
			 */
			template<class Signature, class F>
			void	reg(Key const & key, F && f)
			{
				std::lock_guard<SpinMutex> lock(mutex_);

				LV_ENSURE(invokers_.find(key) == invokers_.end(), "The key has already been used: " + 
						boost::lexical_cast<std::string>(key));

				invokers_.emplace(key, detail::Invoker<Signature, IArchive>(std::forward<F>(f)));
			}


			/**
			 * @exception SerializationError  ( public boost::archive::archive_exception)
			 * @exception runtime_error exceptions thrown by the target function
			 */
			void	invoke(IArchive & ia)
			{
				Key key;
				ia >> key;

				typename invoker_map::iterator it;

				{
					// no deletion, so the iterator will be valid
					std::lock_guard<SpinMutex> lock(mutex_);

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
				catch (boost::archive::archive_exception const & ex)
				{
					throw SerializationError(ex.code, std::string(".error calling function: ") + boost::lexical_cast<std::string>(key));
				}
				catch (std::exception const & ex)
				{
					throw std::runtime_error(ex.what() + std::string(".error calling function: ") + boost::lexical_cast<std::string>(key));
				}
			}

		};
	}

	

} 
