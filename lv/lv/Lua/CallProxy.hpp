// *********************************************************************
//  CallProxy   version:  1.0   ¡¤  date: 2013/07/15
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

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

		CallProxy(std::ostringstream & oss, char const * fn, Callback callback = Callback())
			: oss_(oss)
			, first_param_(true)
			, finished_(false)
			, callback_(std::move(callback))
		{
			oss.str(std::string());
			oss_ << fn << "(";
		}

		CallProxy(CallProxy && other)
			: oss_(other.oss_)
			, first_param_(other.first_param_)
			, callback_(other.callback_)
			, finished_(other.finished_)
		{
			other.callback_ = Callback();
			other.finished_ = true;
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
