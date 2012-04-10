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

#include <lv/Foreach.hpp>
#include <lv/Endian.hpp>
#include <lv/BinaryStream/Tags.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/serialization/serialization.hpp>


namespace lv { namespace bstream {
	
	// fwd
	template<typename T, class Tag>
	struct Serializer;


	template<typename T, class OStream>
	void	write(OStream & os, typename boost::call_traits<T>::param_type t)
	{
		Serializer<T, typename object_tag<T>::type>::write(os, t);
	}

	template<typename T, class IStream>
	void	read(IStream & is, T & t)
	{
		Serializer<T, typename object_tag<T>::type>::read(is, t);
	}


	// serialization

	template<typename T>
	struct Serializer<T, unknown_tag>
	{
		template<class OStream>
		static void write(OStream & os, T const & t)
		{
			boost::serialization::serialize(os, const_cast<T&>(t), 1);
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			boost::serialization::serialize(is, t, 1);
		}
	};

	template<typename T>
	struct Serializer<T, primitive_tag>
	{
		template<class OStream>
		static void write(OStream & os, T t)
		{
			if(os.switch_endian())
			{
				t = lv::endian_switch(t);
			}
				
			os.write(reinterpret_cast<char const*>(&t), sizeof(T));
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			is.read(reinterpret_cast<char*>(&t), sizeof(T));

			if(is.switch_endian())
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
			if(! boost::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if(os.switch_endian() && value_size != 1)
				{
					Serializer<T, range_tag>::write(os, t);
				}
				else
				{
					os.write(reinterpret_cast<char const*>(&*boost::begin(t)), static_cast<std::streamsize>(
						boost::size(t) * value_size));
				}
			}
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			if(! boost::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if(is.switch_endian() && value_size != 1)
				{
					Serializer<T, range_tag>::read(is, t);
				}
				else
				{
					is.read(reinterpret_cast<char*>(&*boost::begin(t)), static_cast<std::streamsize>(
						boost::size(t) * sizeof(typename boost::range_value<T>::type)));
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
			foreach(typename boost::range_value<T>::type const & item, t)
			{
				os << item;
			}
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			foreach(typename boost::range_value<T>::type & item, t)
			{
				is >> item;
			}
		}
	};


} }

#endif
