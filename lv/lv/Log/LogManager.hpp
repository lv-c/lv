// *********************************************************************
//  LogManager   version:  1.0   ¡¤  date: 10/03/2008
//  --------------------------------------------------------------------
//  A simple logger manager.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_LOGMANAGER_HPP
#define LV_LOG_LOGMANAGER_HPP

#include <string>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/thread/mutex.hpp>

#include <lv/Singleton.hpp>
#include <lv/Log/Log.hpp>

namespace lv { namespace log {
	
	class LogManager : public Singleton<LogManager, true>
	{
		typedef boost::mutex::scoped_lock	scoped_lock; 
		boost::mutex mutex_;

		/// note : Log is not copyable, so we can't use std::map here
		typedef boost::ptr_map<std::string, Log>	logger_map;
		logger_map	loggers_;

	public:

		/**
		 * get the static logger
		 */
		Log & logger()
		{
			// scoped_lock lock(mutex_);

			static Log log;		// not thread-safe until C++0x ?
			return log;
		}

		/**
		 * creates a new logger if no logger with the given name exists, otherwise returns 
		 *	the existing logger.
		 */
		Log & create_logger(std::string const & name)
		{
			scoped_lock lock(mutex_);

			return loggers_[name];
		}

		/**
		 * retrieve a logger
		 * @exception std::runtime_error if no logger with the given name exists.
		 */
		Log & logger(std::string const & name)
		{
			scoped_lock lock(mutex_);

			logger_map::iterator it = loggers_.find(name);
			if(it == loggers_.end())
			{
				throw std::runtime_error(std::string("logger not exist : ") + name);
			}

			return *it->second;
		}

	};
} }

#endif
