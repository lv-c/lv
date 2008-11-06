// *********************************************************************
//  BinaryOStream   version:  1.0   ¡¤  date: 10/27/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYOSTREAM_HPP
#define LV_BINARYOSTREAM_HPP

#include <boost/mpl/bool.hpp>

#include <lv/BinaryStream/Serialize.hpp>
#include <lv/StreamPtr.hpp>

namespace lv
{
	class BinaryOStream
	{
		OStreamPtr	ostream_;

	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;

		typedef std::char_traits<char>	traits;

		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios::badbit | std::ios::failbit | std::ios::eofbit
		 * @exception std::ios::failure if the stream passed in has an error state
		 */
		BinaryOStream(OStreamPtr & os)
			: ostream_(os)
		{
			ostream_->exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
		}


		inline BinaryOStream & write(char const * buf, std::streamsize size)
		{
			ostream_->fill()
			ostream_->write(buf, size);
			return *this;
		}


		/**
		 * @exception std::ios::failure
		 */
		template<typename T>
		BinaryOStream & operator << (T const & val)
		{
			bstream::write(*this, val);
			return *this;
		}

		template<typename T>
		BinaryOStream & operator & (T const & val)
		{
			return *this << val;
		}

	};
}

#endif // LV_BINARYOSTREAM_HPP