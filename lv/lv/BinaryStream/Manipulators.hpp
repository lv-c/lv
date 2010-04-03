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
#include <lv/BinaryStream/String.hpp>
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
					throw ce.ex_;

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

	// ignore 

	class ignore
	{
		std::streamsize	size_;

		int	metadelim_;

	public:

		ignore(std::streamsize sz, int metadelim = std::char_traits<char>::eof()) 
			: size_(sz) 
			, metadelim_(metadelim)
		{
		}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, ignore const & ig)
		{
			is.ignore(ig.size_, ig.metadelim_);
			return is;
		}
	};

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
		std::streamsize	size_;

		char	char_;

	public:
		
		fill_n(std::streamsize size, char c) 
			: size_(size) 
			, char_(c)
		{
		}

		friend BinaryOStream & operator << (BinaryOStream & os, fill_n const & fill)
		{
			std::streamsize const buf_size = 128;
			char buf[buf_size];

			std::fill_n(buf, std::min(buf_size, fill.size_), fill.char_);

			std::streamsize left = fill.size_;

			while(left > 0)
			{
				std::streamsize sz = std::min(left, buf_size);

				os.write(buf, sz);

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
			fixed.str_.assign(fixed.size_, '\0');
			is >> fixed.str_;

			fixed.str_ = fixed.str_.c_str();

			return is;
		}

	};

} }



#endif // LV_BINARYSTREAM_MANIPULATORS_HPP