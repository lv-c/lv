// *********************************************************************
//  LogManager   version:  1.0   ¡¤  date: 10/03/2008
//  --------------------------------------------------------------------
//  A simple logger manager.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Singleton.hpp>
#include <lv/Log/Log.hpp>

#include <string>
#include <memory>
#include <map>
#include <mutex>


namespace lv::log
{
	class LogManager : public Singleton<LogManager, true>
	{
		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex mutex_;

		using log_ptr = std::unique_ptr<Log>;
		using logger_map = std::map<std::string, log_ptr>;

		logger_map	loggers_;

	public:

		/**
		 * get the static logger
		 */
		Log &	logger()
		{
			// lock_guard lock(mutex_);

			static Log log;		// not thread-safe until C++0x ?
			return log;
		}

		/**
		 * creates a new logger if no logger with the given name exists, otherwise returns 
		 *	the existing logger.
		 */
		Log &	create_logger(std::string const & name)
		{
			lock_guard lock(mutex_);

			log_ptr & log = loggers_[name];
			if (!log)
			{
				log = std::make_unique<Log>();
			}

			return *log;
		}

		/**
		 * retrieve a logger
		 * @exception std::runtime_error if no logger with the given name exists.
		 */
		Log &	logger(std::string const & name)
		{
			lock_guard lock(mutex_);

			auto it = loggers_.find(name);
			if (it == loggers_.end())
			{
				throw std::runtime_error(std::string("logger not exist : ") + name);
			}

			return *it->second;
		}

	};
}
