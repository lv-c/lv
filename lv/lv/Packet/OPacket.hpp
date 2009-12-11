// *********************************************************************
//  OPacket   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//	It's in fact an portable binary output archive for boost::serialization
//	library. This's a modification of the portable_binary_oarchive 
//	implementation made by christian.pfligersdorffer@eos.info. Here is the 
//	major changes:
//	1. Only write the size of the objects to the stream when LV_PACKET_DEBUG 
//		is defined.
//
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_OPACKET_HPP
#define LV_OPACKET_HPP

#include <boost/assert.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/impl/basic_binary_oprimitive.ipp>
#include <boost/archive/impl/basic_binary_oarchive.ipp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <boost/utility/enable_if.hpp>

/*
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <lv/Buffer.hpp>
*/


namespace lv
{

	class OPacket : public boost::archive::binary_oarchive_impl<
		OPacket, std::ostream::char_type, std::ostream::traits_type>
	{

		// boost::iostreams::filtering_ostream buf_ostream_;

		typedef boost::archive::binary_oarchive_impl<OPacket, 
			std::ostream::char_type, std::ostream::traits_type>	archive_base_t;


		friend class boost::archive::basic_binary_oprimitive<OPacket, std::ostream::char_type,
			std::ostream::traits_type>;
		friend class boost::archive::save_access;
		
		void	save_size(char size)
		{
#if	LV_PACKET_DEBUG
			archive_base_t::save(size);
#endif
		}

		template<class T>
		typename boost::disable_if<boost::is_arithmetic<T> >::type save(T const & t)
		{
			archive_base_t::save(t);
		}


		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type save(T const & t)
		{
			save_size(sizeof(T));

			save_binary(&t, sizeof(T));
		}

		void	save(bool const & b)
		{
			save_size(sizeof(char));
			archive_base_t::save<char>(b);
		}


	public:

		OPacket(std::ostream & os, unsigned int flags = 0)
			: archive_base_t(os, flags | boost::archive::no_header | boost::archive::no_codecvt)
		{
		}

		OPacket(std::streambuf & buf, unsigned int flags = 0)
			: archive_base_t(buf, flags | boost::archive::no_header | boost::archive::no_codecvt)
		{
		}

		/*
		// bug
		OPacket(Buffer & buffer, unsigned int flags = 0)
			: buf_ostream_(boost::iostreams::back_inserter(buffer))
			, archive_base_t(buf_ostream_, flags | boost::archive::no_header)
		{
		}*/

	};

}


#endif // LV_OPACKET_HPP