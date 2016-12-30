// *********************************************************************
//  Serializer   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_SERIALIZER_HPP
#define LV_BINARYSTREAM_SERIALIZER_HPP

#include <lv/Endian.hpp>
#include <lv/BinaryStream/Tags.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/range/value_type.hpp>


namespace lv { namespace bstream {
	
	// fwd
	template<typename T, class Tag>
	struct Serializer;


	template<typename T, class OStream>
	void	write(OStream & os, T const & t)
	{
		Serializer<T, object_tag_t<T> >::write(os, t);
	}

	template<typename T, class IStream>
	void	read(IStream & is, T & t)
	{
		typedef object_tag_t<std::decay_t<T> >	tag_type;
		Serializer<T, tag_type>::read(is, t);
	}

	template<typename T, class IStream>
	void	read(IStream & is, T const & t)
	{
		Serializer<T const, object_tag_t<T> >::read(is, t);
	}


	// serialization

	template<typename T>
	struct Serializer<T, unknown_tag>
	{
		template<class OStream>
		static void write(OStream & os, T const & t)
		{
			boost::serialization::serialize_adl(os, const_cast<T&>(t), 1);
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			boost::serialization::serialize_adl(is, t, 1);
		}
	};

	template<typename T>
	struct Serializer<T, primitive_tag>
	{
		template<class OStream>
		static void write(OStream & os, T t)
		{
			if (os.switch_endian())
			{
				t = lv::endian_switch(t);
			}
				
			os.write(reinterpret_cast<char const*>(&t), sizeof(T));
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			is.read(reinterpret_cast<char*>(&t), sizeof(T));

			if (is.switch_endian())
			{
				t = lv::endian_switch(t);
			}
		}
	};


	template<typename T>
	struct Serializer<T, primitive_buffer_tag>
	{
		template<class OStream>
		static void write(OStream & os, T const & t)
		{
			if (! std::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if (os.switch_endian() && value_size != 1)
				{
					Serializer<T, range_tag>::write(os, t);
				}
				else
				{
					os.write(reinterpret_cast<char const*>(&*std::begin(t)), static_cast<std::streamsize>(
						std::size(t) * value_size));
				}
			}
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			if (! std::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if (is.switch_endian() && value_size != 1)
				{
					Serializer<T, range_tag>::read(is, t);
				}
				else
				{
					is.read(reinterpret_cast<char*>(&*std::begin(t)), static_cast<std::streamsize>(
						std::size(t) * sizeof(typename boost::range_value<T>::type)));
				}
			}
		}
	};


	template<typename T>
	struct Serializer<T, range_tag>
	{
		template<class OStream>
		static void write(OStream & os, T const & t)
		{
			for (auto const & item : t)
			{
				os << item;
			}
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			for (auto & item : t)
			{
				is >> item;
			}
		}
	};


} }

#endif
