// *********************************************************************
//  Manipulators   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_MANIPULATORS_HPP
#define LV_BINARYSTREAM_MANIPULATORS_HPP

#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/BinaryOStream.hpp>
#include <lv/Exception.hpp>

namespace lv { namespace bstream {


	// check_equal

	DEFINE_EXCEPTION_MSG(CheckEqualError, std::runtime_error);

	
	namespace detail
	{
		template<typename T, class Except>
		class check_equal_impl
		{
			T	t_;

			Except	ex_;

		public:
			check_equal_impl(T const & t, Except const & ex) 
				: t_(t) 
				, ex_(ex)
			{
			}

			/**
			 * @exception CheckEqualError
			 */
			friend BinaryIStream & operator >> (BinaryIStream & is, check_equal_impl const & ce)
			{
				T val;
				is >> val;
				if(val != ce.t_)
				{
					throw ce.ex_;
				}

				return is;
			}
		};
	}

	template<typename T>
	inline detail::check_equal_impl<T, CheckEqualError>	check_equal(T const & t)
	{
		return detail::check_equal_impl<T, CheckEqualError>(t, CheckEqualError());
	}
	
	// you can specify your own exception type
	template<typename T, class Except>
	inline detail::check_equal_impl<T, Except>	check_equal(T const & t, Except const & ex)
	{
		return detail::check_equal_impl<T, Except>(t, ex);
	}

	// forward

	class forward
	{
		std::streamoff off_;

	public:
		
		forward(std::streamoff off) : off_(off) {}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, forward const & fwd)
		{
			is.seekg(fwd.off_, std::ios_base::cur);
			return is;
		}

	};

	//
	class fill_n
	{
		std::size_t	size_;

		char	char_;

	public:
		
		fill_n(std::size_t size, char c) 
			: size_(size) 
			, char_(c)
		{
		}

		friend BinaryOStream & operator << (BinaryOStream & os, fill_n const & fill)
		{
			std::size_t const buf_size = 128;
			char buf[buf_size];

			std::fill_n(buf, std::min(buf_size, fill.size_), fill.char_);

			std::size_t left = fill.size_;

			while(left > 0)
			{
				std::size_t sz = std::min(left, buf_size);

				os.write(buf, static_cast<std::streamsize>(sz));

				left -= sz;
			}

			return os;
		}
	};

	//

	class fixed_len_str
	{
		std::string	&	str_;

		std::size_t		size_;

	public:

		fixed_len_str(std::string & str, std::size_t size) 
			: str_(str)
			, size_(size) 
		{
		}

		friend BinaryIStream & operator >> (BinaryIStream & is, fixed_len_str const & fixed)
		{
			std::string & str = fixed.str_;
			std::size_t size = fixed.size_;

			str.resize(size);

			if(size > 0)
			{
				is.read(&str[0], static_cast<std::streamsize>(size));

				for(size_t i = 0; i < size; ++i)
				{
					if(str[i] == '\0')
					{
						str.resize(i);
						break;
					}
				}
			}

			return is;
		}

		friend BinaryOStream & operator << (BinaryOStream & os, fixed_len_str const & fixed)
		{
			BOOST_ASSERT(fixed.str_.size() <= fixed.size_);

			std::size_t size = std::min(fixed.str_.size(), fixed.size_);
			if(size > 0)
			{
				os.write(&fixed.str_[0], static_cast<std::streamsize>(size));
			}

			if(size < fixed.size_)
			{
				os << fill_n(fixed.size_ - size, '\0');
			}
		}
	};


	namespace detail
	{
		template<typename SizeType, class Range>
		class variable_len_range_impl
		{
			Range &	range_;

		public:

			explicit variable_len_range_impl(Range & range)
				: range_(range)
			{
			}

			friend BinaryIStream & operator >> (BinaryIStream & is, variable_len_range_impl const & var)
			{
				SizeType size;
				is >> size;

				var.range_.resize(size);
				is >> var.range_;

				return is;
			}

			friend BinaryOStream & operator << (BinaryOStream & os, variable_len_range_impl const & var)
			{
				SizeType size = static_cast<SizeType>(var.range_.size());
				os << size << var.range_;

				return os;
			}
		};
	}

	template<typename SizeType, class Range>
	detail::variable_len_range_impl<SizeType, Range>	variable_len_range(Range & range)
	{
		return detail::variable_len_range_impl<SizeType, Range>(range);
	}

} }


#endif