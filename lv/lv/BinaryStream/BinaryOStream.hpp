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

#include <lv/BinaryStream/BinaryStreamBase.hpp>
#include <lv/BinaryStream/Serializer.hpp>
#include <lv/Stream/OStreamProxy.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/optional.hpp>
#include <boost/ref.hpp>

namespace lv
{
	class BinaryOStream : public BinaryStreamBase, public OStreamProxy
	{

		boost::optional<OBufferStream>	raw_os_;

	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;


		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios_base::badbit | std::ios_base::failbit
		 * @exception std::ios_base::failure if the stream passed in has an error state
		 */
		explicit BinaryOStream(std::ostream & os)
			: OStreamProxy(os)
		{
			set_exceptions();
		}

		explicit BinaryOStream(Buffer & buf)
			: raw_os_(boost::ref(buf))
		{
			OStreamProxy::set(nullptr, raw_os_.get_ptr());

			set_exceptions();
		}

		explicit BinaryOStream(BufferPtr buf)
			: raw_os_(buf)
		{
			OStreamProxy::set(nullptr, raw_os_.get_ptr());

			set_exceptions();
		}

		Buffer &	buffer()
		{
			return raw_os_->buffer();
		}

		BufferPtr	buffer_ptr()
		{
			return raw_os_->buffer_ptr();
		}

		/**
		 * @exception std::ios_base::failure
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

	private:

		void	set_exceptions()
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
		}

	};
}

#endif
