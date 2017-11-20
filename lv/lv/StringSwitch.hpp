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

#include <lv/Ensure.hpp>

#include <boost/assert.hpp>

#include <string_view>


// see llvm.StringSwitch

namespace lv
{
	template<class T>
	class StringSwitch
	{
		std::string_view	str_;

		T const *			result_;

	public:

		explicit StringSwitch(std::string_view str)
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

		StringSwitch &	case_(std::string_view s, T const & value)
		{
			if (result_ == nullptr && str_ == s)
			{
				result_ = &value;
			}

			return *this;
		}


		template<size_t N>
		StringSwitch &	starts_with(char const (&s)[N], T const & value)
		{
			return starts_with_impl(s, N - 1, value);
		}

		StringSwitch &	starts_with(std::string_view s, T const & value)
		{
			return starts_with_impl(s.data(), s.size(), value);
		}


		template<size_t N>
		StringSwitch &	ends_with(char const (&s)[N], T const & value)
		{
			return ends_with_impl(s, N - 1, value);
		}

		StringSwitch &	ends_with(std::string_view s, T const & value)
		{
			return ends_with_impl(s.data(), s.size(), value);
		}


		template<class E>
		StringSwitch &	except(E const & ex)
		{
			LV_ENSURE(result_ != nullptr, ex);
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

	private:

		StringSwitch &	starts_with_impl(char const * s, size_t size, T const & value)
		{
			if (result_ == nullptr && str_.size() >= size &&
				std::memcmp(str_.data(), s, size) == 0)
			{
				result_ = &value;
			}

			return *this;
		}

		StringSwitch &	ends_with_impl(char const * s, size_t size, T const & value)
		{
			if (result_ == nullptr && str_.size() >= size &&
				std::memcmp(str_.data() + str_.size() - size, s, size) == 0)
			{
				result_ = &value;
			}

			return *this;
		}
	};
}

#endif