// *********************************************************************
//  StringSwitch   version:  1.0   ¡¤  date: 2017/05/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2017 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STRINGSWITCH_HPP
#define LV_STRINGSWITCH_HPP

#include <boost/utility/string_view.hpp>
#include <boost/assert.hpp>


// see llvm.StringSwitch

namespace lv
{
	template<class T>
	class StringSwitch
	{
		boost::string_view	str_;

		T const *			result_;

	public:

		explicit StringSwitch(boost::string_view str)
			: str_(str)
			, result_(nullptr)
		{
		}

		template<size_t N>
		StringSwitch &	case_(char const (&s)[N], T const & value)
		{
			if (result_ == nullptr && str_.size() == N - 1 &&
				std::memcmp(str_.data(), s, N - 1) == 0)
			{
				result_ = &value;
			}

			return *this;
		}

		template<size_t N>
		StringSwitch &	starts_with(char const (&s)[N], T const & value)
		{
			if (result_ == nullptr && str_.size() >= N - 1 &&
				std::memcmp(str_.data(), s, N - 1))
			{
				result_ = &value;
			}

			return *this;
		}

		template<size_t N>
		StringSwitch &	ends_with(char const (&s)[N], T const & value)
		{
			if (result_ == nullptr && str_.size() >= N - 1 &&
				std::memcmp(str_.data() + str_.size() + 1 - N, s, N - 1) == 0)
			{
				result_ = &value;
			}

			return *this;
		}

		T	default_(T const & value) const
		{
			if (result_ != nullptr)
			{
				return *result_;
			}

			return value;
		}

		operator T ()	const
		{
			BOOST_ASSERT(result_);
			return *result_;
		}
	};
}

#endif