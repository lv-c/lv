// *********************************************************************
//  CallProxy   version:  1.0   ¡¤  date: 2013/07/15
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUA_CALLPROXY_HPP
#define LV_LUA_CALLPROXY_HPP

#include <boost/function.hpp>

#include <sstream>

namespace lv { namespace lua {

	class CallProxy
	{
		std::ostringstream &	oss_;

		bool	first_param_;

		typedef boost::function<void(std::string const &)> Callback;

		Callback	callback_;

	public:

		CallProxy(std::ostringstream & oss, char const * fun, Callback const & callback)
			: oss_(oss)
			, first_param_(true)
			, callback_(callback)
		{
			oss.str(std::string());
			oss_ << fun << "(";
		}

		CallProxy(CallProxy & rhs)
			: oss_(rhs.oss_)
			, first_param_(rhs.first_param_)
			, callback_(rhs.callback_)
		{
			rhs.callback_ = Callback();
		}

		~CallProxy()
		{
			if(callback_)
			{
				oss_ << ")";
				callback_(oss_.str());
			}
		}

		template<class T>
		CallProxy & operator << (T const & t)
		{
			if(first_param_)
			{
				first_param_ = false;
			}
			else
			{
				oss_ << ", ";
			}

			LuaOArchive oa(oss_);
			oa << t;

			return *this;
		}
	};

} }

#endif
