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

#include <lv/BinaryStream/Fwd.hpp>
#include <lv/BinaryStream/BinaryStreamBase.hpp>
#include <lv/BinaryStream/ISerializer.hpp>
#include <lv/Stream/IStreamProxy.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/optional.hpp>


namespace lv::bstream
{
	class BinaryIStream : public BinaryStreamBase, public IStreamProxy
	{

		boost::optional<IBufferStream>	raw_is_;

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
			set_exceptions();
		}

		explicit BinaryIStream(ConstBufferRef const & buf)
			: raw_is_(buf)
		{
			IStreamProxy::set(nullptr, raw_is_.get_ptr());

			set_exceptions();
		}

		BinaryIStream(BinaryIStream && other)
			: BinaryStreamBase(std::move(other))
			, IStreamProxy(std::move(other))
			, raw_is_(std::move(other.raw_is_))
		{
			if (raw_is_)
			{
				IStreamProxy::set(nullptr, raw_is_.get_ptr());
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
		template<typename T>
		BinaryIStream & operator >> (T && val)
		{
			bstream::load(*this, val);
			return *this;
		}

		template<typename T>
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

#endif
