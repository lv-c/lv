// *********************************************************************
//  OPacket   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//	It's in fact an portable binary output archive for boost::serialization
//	library. This's a modification of the portable_binary_oarchive 
//	implementation made by christian.pfligersdorffer@eos.info. Here is the 
//	major changes:
//	1. Only write the size of the objects to the stream when LV_DEBUG_PACKET 
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
#include <boost/archive/detail/polymorphic_oarchive_route.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <boost/utility/enable_if.hpp>

// TODO: replace these with something else.
#include <boost/spirit/home/support/detail/math/fpclassify.hpp>
#include <boost/spirit/home/support/detail/integer/endian.hpp>	

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <lv/Buffer.hpp>

namespace lv
{

	class OPacket : public boost::archive::binary_oarchive_impl<
		OPacket, std::ostream::char_type, std::ostream::traits_type>
	{

		boost::iostreams::filtering_ostream buf_ostream_;

	private:

		typedef boost::archive::binary_oarchive_impl<OPacket, 
			std::ostream::char_type, std::ostream::traits_type>	archive_base_t;


		void	save_size(char size)
		{
#ifdef LV_DEBUG_PACKET
			archive_base_t::save(size);
#endif
		}

		/*

		friend class boost::archive::detail::polymorphic_oarchive_route<OPacket>;
		friend class boost::archive::basic_binary_oarchive<OPacket>;
		friend class boost::archive::save_access;
		*/
		
		template<class T>
		typename boost::disable_if<boost::is_arithmetic<T> >::type save(T const & t)
		{
			archive_base_t::save(t);
		}


		template<typename T>
		typename boost::enable_if<boost::is_integral<T> >::type save(T const & t)
		{
			save_size(sizeof(T));

			T temp;
			boost::detail::store_little_endian(<T, sizeof(T)>(&temp, t));
			save_binary(&temp, sizeof(T));
		}

		void	save(bool const & b)
		{
			save_size(sizeof(char));
			archive_base_t::save<char>(b);
		}

		template<typename T>
		typename boost::enable_if<boost::is_floating_point<T> >::type save(T const & t)
		{
			/*
			using namespace boost::math::detail;
			typename fp_traits<T>::type::bits bits;
			fp_traits<T>::type::get_bits(t, bits);
			save(bits);
			*/

			BOOST_ASSERT(false && "DO NOT use this at the moment");
			throw PacketException();
		}

	public:

		OPacket(std::ostream & os, unsigned int flags = 0)
			: archive_base_t(os, flags | boost::archive::no_header)
		{
		}

		OPacket(Buffer & buffer, unsigned int flags = 0)
			: buf_ostream_(boost::iostreams::back_inserter(buffer))
			, archive_base_t(buf_ostream_, flags | boost::archive::no_header)
		{
		}

	};

}


#endif // LV_OPACKET_HPP