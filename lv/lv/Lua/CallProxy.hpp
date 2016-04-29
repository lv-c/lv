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

		bool	finished_;

		typedef boost::function<void(std::string const &)> Callback;

		Callback	callback_;

	public:

		CallProxy(std::ostringstream & oss, char const * fun, Callback const & callback = Callback())
			: oss_(oss)
			, first_param_(true)
			, finished_(false)
			, callback_(callback)
		{
			oss.str(std::string());
			oss_ << fun << "(";
		}

		CallProxy(CallProxy & rhs)
			: oss_(rhs.oss_)
			, first_param_(rhs.first_param_)
			, callback_(rhs.callback_)
			, finished_(rhs.finished_)
		{
			rhs.callback_ = Callback();
			rhs.finished_ = true;
		}

		~CallProxy()
		{
			if (callback_)
			{
				if (! finished_)
				{
					finish();
					callback_(oss_.str());
				}
				else
				{
					BOOST_ASSERT(false);
				}
			}
		}

		operator std::string ()
		{
			if (! finished_)
			{
				finish();
				return oss_.str();
			}
			else
			{
				BOOST_ASSERT(false);
				return std::string();
			}
		}

		template<class T>
		CallProxy & operator << (T const & t)
		{
			BOOST_ASSERT(! finished_);

			if (first_param_)
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

	private:
		
		void	finish()
		{
			oss_ << ")";
			finished_ = true;
		}
	};

} }

#endif
