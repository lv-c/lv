// *********************************************************************
//  BinaryOStream   version:  1.0   ��  date: 10/27/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYOSTREAM_HPP
#define LV_BINARYOSTREAM_HPP

#include <lv/Buffer.hpp>
#include <lv/BinaryStream/Serialize.hpp>
#include <lv/StreamPtr.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>

namespace lv
{
	class BinaryOStream
	{
		OStreamPtr	ostream_;

		BufferPtr	buf_;

	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;

		typedef std::char_traits<char>	traits;

		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios::badbit | std::ios::failbit
		 * @exception std::ios::failure if the stream passed in has an error state
		 */
		explicit BinaryOStream(OStreamPtr os)
			: ostream_(os)
		{
			ostream_->exceptions(std::ios::badbit | std::ios::failbit);
		}

		explicit BinaryOStream(BufferPtr buf) : buf_(buf) {}

		OStreamPtr	ostream() const
		{
			return ostream_;
		}

		BufferPtr	buffer() const
		{
			return buf_;
		}


		inline BinaryOStream & write(char const * buf, std::streamsize size)
		{
			if(ostream_)
				ostream_->write(buf, size);
			else
				buffer::write(*buf_, buf, size);
			return *this;
		}


		/**
		 * @exception std::ios::failure
		 */
		template<typename T>
		BinaryOStream & operator << (T const & val)
		{
			bstream::write<T>(*this, val);
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
