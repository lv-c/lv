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

		typedef BinaryIStream::traits::int_type	int_type;

		int_type	metadelim_;
	public:

		ignore(std::streamsize sz, int_type metadelim = BinaryIStream::traits::eof()) 
			: size_(sz) 
			, metadelim_(metadelim)
		{
		}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, ignore const & sk)
		{
			return is.ignore(sk.size_, sk.metadelim_);
		}
	};

	// forward

	class forward
	{
		typedef BinaryIStream::traits::off_type	off_type;
		off_type off_;

	public:
		
		forward(off_type off) : off_(off) {}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, forward const & fwd)
		{
			return is.seekg(fwd.off_, std::ios_base::cur);
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

} }



#endif // LV_BINARYSTREAM_MANIPULATORS_HPP