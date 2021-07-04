// *********************************************************************
//  BinaryOStream   version:  1.0   ¡¤  date: 10/27/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/Fwd.hpp>
#include <lv/BinaryStream/BinaryStreamBase.hpp>
#include <lv/BinaryStream/OSerializer.hpp>
#include <lv/Stream/OStreamProxy.hpp>

#include <boost/mpl/bool.hpp>

#include <optional>


namespace lv::bstream
{
	class BinaryOStream : public BinaryStreamBase, public OStreamProxy
	{

		std::optional<OBufferStream>	raw_os_;

	public:

		using is_saving = boost::mpl::true_;
		using is_loading = boost::mpl::false_;


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
			: raw_os_(std::ref(buf))
		{
			OStreamProxy::set(nullptr, &(*raw_os_));

			set_exceptions();
		}

		explicit BinaryOStream(BufferPtr buf)
			: raw_os_(buf)
		{
			OStreamProxy::set(nullptr, &(*raw_os_));

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
		template<class T>
		BinaryOStream & operator << (T const & val)
		{
			bstream::save(*this, val);
			return *this;
		}

		template<class T>
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
