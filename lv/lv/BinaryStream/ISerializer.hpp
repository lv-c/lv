// *********************************************************************
//  ISerializer   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/Tags.hpp>
#include <lv/Endian.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/range/value_type.hpp>


namespace lv::bstream
{
	namespace detail
	{
		// unknown_tag
		template<class IStream, class T>
		void	load_impl(IStream & is, T & t, unknown_tag)
		{
			boost::serialization::serialize_adl(is, t, 1);
		}

		// primitive_tag
		template<class IStream, class T>
		void load_impl(IStream & is, T & t, primitive_tag)
		{
			is.read(reinterpret_cast<char*>(&t), sizeof(T));

			if (is.switch_endian())
			{
				t = lv::endian_switch(t);
			}
		}

		// range_tag
		template<class IStream, class T>
		void	load_impl(IStream & is, T & t, range_tag)
		{
			for (auto & item : t)
			{
				is >> item;
			}
		}

		// primitive_buffer_tag
		template<class IStream, class T>
		void	load_impl(IStream & is, T & t, primitive_buffer_tag)
		{
			if (!std::empty(t))
			{
				constexpr size_t value_size = sizeof(typename boost::range_value<T>::type);

				if (is.switch_endian() && value_size != 1)
				{
					load_impl(is, t, range_tag());
				}
				else
				{
					is.read(reinterpret_cast<char*>(&*std::begin(t)), static_cast<std::streamsize>(
						std::size(t) * value_size));
				}
			}
		}
	}


	template<class IStream, class T>
	void	load(IStream & is, T & t)
	{
		using tag_type = object_tag<std::remove_cv_t<T> >;
		detail::load_impl(is, t, tag_type());
	}

}
