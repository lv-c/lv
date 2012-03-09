// *********************************************************************
//  BinaryIStream   version:  1.0   ·  date: 10/26/2008
//  --------------------------------------------------------------------
//  binary stream : 二进制流。主要用于二进制文件的读写，　比如　bmp, wav 
//	文件等。　跟序列化(如 boost::serialization) 最大的区别是，序列化的文件
//	格式是自定义的，　一般要包括一些额外信息，　如果数组的长度等；而这个库
//	的主要目的是读(写)用户定义好的格式
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYISTREAM_HPP
#define LV_BINARYISTREAM_HPP

#include <lv/BinaryStream/BinaryStreamBase.hpp>
#include <lv/BinaryStream/Serializer.hpp>
#include <lv/Stream/IStreamProxy.hpp>

#include <boost/mpl/bool.hpp>

namespace lv
{
	class BinaryIStream : public BinaryStreamBase, public IStreamProxy
	{
	public:
		
		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;


		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios_base::badbit | std::ios_base::failbit
		 * @exception std::ios_base::failure if the stream passed in has an error state
		 */
		explicit BinaryIStream(std::istream & is)
			: IStreamProxy(is)
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
		}

		explicit BinaryIStream(IBufferStream & is)
			: IStreamProxy(is)
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
		}


		/**
		 * @exception std::ios_base::failure
		 */
		template<typename T>
		BinaryIStream & operator >> (T & val)
		{
			bstream::read(*this, val);
			return *this;
		}

		template<typename T>
		BinaryIStream & operator & (T & val)
		{
			return *this >> val;
		}

	};
}

#endif // LV_BINARYISTREAM_HPP
