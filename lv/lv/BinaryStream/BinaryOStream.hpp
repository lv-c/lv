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

#include <lv/BinaryStream/Serializer.hpp>
#include <lv/Stream/OStreamProxy.hpp>

#include <boost/mpl/bool.hpp>

namespace lv
{
	class BinaryOStream : public OStreamProxy
	{
	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;


		/**
		 * constructor. sets the exception mask of the stream to 
		 *		std::ios_base::badbit | std::ios_base::failbit
		 * @exception std::ios_base::failure if the stream passed in has an error state
		 */
		BinaryOStream(std::ostream & os)
			: OStreamProxy(os)
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
		}

		BinaryOStream(OBufferStream & os)
			: OStreamProxy(os)
		{
			exceptions(std::ios_base::badbit | std::ios_base::failbit);
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

	};
}

#endif // LV_BINARYOSTREAM_HPP
