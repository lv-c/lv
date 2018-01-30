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

#include <sstream>
#include <functional>


namespace lv::lua
{
	class CallProxy
	{
		std::ostringstream &	oss_;

		bool	first_param_;

		bool	finished_;

		using Callback = std::function<void(std::string const &)>;

		Callback	callback_;

	public:

		CallProxy(std::ostringstream & oss, char const * fn, Callback const & callback = Callback())
			: oss_(oss)
			, first_param_(true)
			, finished_(false)
			, callback_(callback)
		{
			oss.str(std::string());
			oss_ << fn << "(";
		}

		CallProxy(CallProxy && rhs)
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
				if (!finished_)
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
			if (!finished_)
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
			BOOST_ASSERT(!finished_);

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

}

#endif
