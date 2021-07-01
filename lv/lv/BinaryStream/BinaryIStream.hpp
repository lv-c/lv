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

#pragma once

#include <lv/BinaryStream/Fwd.hpp>
#include <lv/BinaryStream/BinaryStreamBase.hpp>
#include <lv/BinaryStream/ISerializer.hpp>
#include <lv/Stream/IStreamProxy.hpp>

#include <boost/mpl/bool.hpp>

#include <optional>


namespace lv::bstream
{
	class BinaryIStream : public BinaryStreamBase, public IStreamProxy
	{

		std::optional<IBufferStream>	raw_is_;

	public:
		
		using is_loading = boost::mpl::true_;
		using is_saving = boost::mpl::false_;


		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios_base::badbit | std::ios_base::failbit
		 * @exception std::ios_base::failure if the stream passed in has an error state
		 */
		explicit BinaryIStream(std::istream & is)
			: IStreamProxy(is)
		{
			set_exceptions();
		}

		explicit BinaryIStream(ConstBufferRef const & buf)
			: raw_is_(buf)
		{
			IStreamProxy::set(nullptr, &(*raw_is_));

			set_exceptions();
		}

		BinaryIStream(BinaryIStream && other) noexcept
			: BinaryStreamBase(std::move(other))
			, IStreamProxy(std::move(other))
			, raw_is_(std::move(other.raw_is_))
		{
			if (raw_is_)
			{
				IStreamProxy::set(nullptr, &(*raw_is_));
			}
		}

		streamsize	bytes_left()
		{
			streampos pos = tellg();
			seekg(0, std::ios_base::end);

			streamsize bytes = tellg() - pos;
			seekg(pos);

			return bytes;
		}

		/**
		 * @exception std::ios_base::failure
		 */
		template<class T>
		BinaryIStream & operator >> (T && val)
		{
			bstream::load(*this, val);
			return *this;
		}

		template<class T>
		BinaryIStream & operator & (T && val)
		{
			return *this >> std::forward<T>(val);
		}

	private:

		void	set_exceptions()
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
		}

	};
}
