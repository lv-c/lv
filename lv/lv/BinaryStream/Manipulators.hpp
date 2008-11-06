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
				if(val != t_)
					throw ex_;

				return is;
			}
		};
	}

	template<typename T>
	inline detail::check_equal_impl<T>	check_equal(T const & t)
	{
		return check_equal_impl<T, CheckEqualError>(t, CheckEqualError());
	}
	
	template<typename T, class Except>
	inline detail::check_equal_impl<T>	check_equal(T const & t, Except const & ex)
	{
		return check_equal_impl<T, Except>(t, ex);
	}

	// ignore 

	class ignore
	{
		std::streamsize	size_;

		typedef BinaryIStream::traits::int_type	int_type;

		int_type	metadelim_;
	public:

		ignore(streamsize sz, int_type metadelim = BinaryIStream::traits::eof) 
			: size_(sz) 
			, metadelim_(metadelim)
		{
		}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, ignore const & sk)
		{
			return is.ignore(sk.size_, sk.metadelim_)
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

} }



#endif // LV_BINARYSTREAM_MANIPULATORS_HPP