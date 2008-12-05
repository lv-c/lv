// *********************************************************************
//  IPacket   version:  1.0   ¡¤  date: 09/13/2008
//  --------------------------------------------------------------------
//	See OPacket.hpp
//	
//	'I' means input, not interface.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IPACKET_HPP
#define LV_IPACKET_HPP

#include <istream>

#include <boost/assert.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>
#include <boost/archive/impl/basic_binary_iarchive.ipp>


#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>


#include <boost/utility/enable_if.hpp>

// TODO: replace these with something else.
#include <boost/spirit/home/support/detail/math/fpclassify.hpp>
#include <boost/spirit/home/support/detail/integer/endian.hpp>	

/*
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/range/iterator_range.hpp>

#include <lv/Buffer.hpp>
*/

#include <lv/Packet/PacketException.hpp>


namespace lv
{


	class IPacket : public boost::archive::binary_iarchive_impl<
		IPacket, std::istream::char_type, std::istream::traits_type>
	{
		
		typedef boost::archive::binary_iarchive_impl<IPacket, std::istream::char_type, 
			std::istream::traits_type>	archive_base_t;

		//boost::iostreams::filtering_istream buf_istream_;

		friend class boost::archive::basic_binary_iprimitive<IPacket, std::istream::char_type,
			std::istream::traits_type>;
		friend class boost::archive::load_access;


		void load_size(char size)
		{
#if	LV_PACKET_DEBUG
			char temp;
			archive_base_t::load(temp);
			if(temp != size)
				throw PacketException(PacketException::incompatible_native_format);
#endif
		}

		template<class T>
		typename boost::disable_if<boost::is_arithmetic<T> >::type load(T & t)
		{
			archive_base_t::load(t);
		}

		
		template<typename T>
		typename boost::enable_if<boost::is_integral<T> >::type load(T & t)
		{
			load_size(sizeof(T));
				
			T temp;
			load_binary(&temp, sizeof(T));
			t = boost::detail::load_little_endian<T, sizeof(T)>(&temp);
		}

		void	load(bool & b)
		{
			load_size(sizeof(char));
	
			char c;
			archive_base_t::load(c);
			if(c != 0 && c != 1)
				throw PacketException(PacketException::incompatible_native_format);

			b = (c != 0);
		}


		template<typename T>
		typename boost::enable_if<boost::is_floating_point<T> >::type load(T & t)
		{
			/*
			using namespace boost::math::detail;
			typename fp_traits<T>::type::bits bits;
			load(bits);
			fp_traits<T>:::type::set_bits(t, bits);
			*/

			BOOST_ASSERT(false && "DO NOT use this at the moment");
			throw PacketException();
		}

	public:

		IPacket(std::istream & is, unsigned int flags = 0)
			: archive_base_t(is, flags | boost::archive::no_header)
		{
		}

		/*
		bug
		IPacket(ConstBufferRef buffer, unsigned int flags = 0)
			: buf_istream_(boost::make_iterator_range(buffer.begin(), buffer.end()))
			, archive_base_t(buf_istream_, flags | boost::archive::no_header)
		{
		}
		*/
	};
}

#endif // LV_IPACKET_HPP