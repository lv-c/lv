// *********************************************************************
//  BinaryIStream   version:  1.0   ��  date: 10/26/2008
//  --------------------------------------------------------------------
//  binary stream : ������������Ҫ���ڶ������ļ��Ķ�д�������硡bmp, wav 
//	�ļ��ȡ��������л�(�� boost::serialization) ���������ǣ����л����ļ�
//	��ʽ���Զ���ģ���һ��Ҫ����һЩ������Ϣ�����������ĳ��ȵȣ��������
//	����ҪĿ���Ƕ�(д)�û�����õĸ�ʽ
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYISTREAM_HPP
#define LV_BINARYISTREAM_HPP

#include <boost/mpl/bool.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <lv/StreamPtr.hpp>
#include <lv/BinaryStream/Serialize.hpp>

namespace lv
{
	class BinaryIStream
	{

		IStreamPtr	istream_;

		ConstBufferRef	buf_;

	public:
		
		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;

		typedef std::char_traits<char>	traits;
		typedef traits::off_type	off_type;
		typedef traits::pos_type	pos_type;
		typedef traits::int_type	int_type;

		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios::badbit | std::ios::failbit
		 * @exception std::ios::failure if the stream passed in has an error state
		 */
		explicit BinaryIStream(IStreamPtr is)
			: istream_(is)
		{
			istream_->exceptions(std::ios::badbit | std::ios::failbit);
		}

		explicit BinaryIStream(ConstBufferRef buf)
			: buf_(buf)
			, istream_(new boost::iostreams::stream<boost::iostreams::array_source>(buf.data(), buf.size()))
		{
		}

		IStreamPtr	istream() const
		{
			return istream_;
		}

		ConstBufferRef	buffer() const
		{
			return buf_;
		}

		inline BinaryIStream & read(char * buf, std::streamsize size)
		{
			istream_->read(buf, size);
			return *this;
		}

		inline BinaryIStream & ignore(std::streamsize size, int_type metadelim = traits::eof())
		{
			istream_->ignore(size, metadelim);
			return *this;
		}

		inline BinaryIStream & seekg(pos_type pos)
		{
			istream_->seekg(pos);
			return *this;
		}

		inline BinaryIStream & seekg(off_type off, std::ios_base::seekdir way)
		{
			istream_->seekg(off, way);
			return *this;
		}

		inline pos_type tellg()
		{
			return istream_->tellg();
		}

		/**
		 * @exception std::ios_base::failure
		 */
		template<typename T>
		inline BinaryIStream & operator >> (T & val)
		{
			bstream::read(*this, val);
			return *this;
		}

		template<typename T>
		inline BinaryIStream & operator & (T & val)
		{
			return *this >> val;
		}

	};
}

#endif // LV_BINARYISTREAM_HPP