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

#include <lv/BinaryStream/Fwd.hpp>
#include <lv/BinaryStream/Tags.hpp>
#include <lv/Endian.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/range/value_type.hpp>


namespace lv { namespace bstream {

	namespace detail
	{
		// unknown_tag
		template<typename T>
		void	save_impl(BinaryOStream & os, T const & t, unknown_tag)
		{
			boost::serialization::serialize_adl(os, const_cast<T&>(t), 1);
		}

		template<typename T>
		void	load_impl(BinaryIStream & is, T & t, unknown_tag)
		{
			boost::serialization::serialize_adl(is, t, 1);
		}

		// primitive_tag
		template<typename T>
		void	save_impl(BinaryOStream & os, T t, primitive_tag)
		{
			if (os.switch_endian())
			{
				t = lv::endian_switch(t);
			}

			os.write(reinterpret_cast<char const*>(&t), sizeof(T));
		}

		template<typename T>
		void load_impl(BinaryIStream & is, T & t, primitive_tag)
		{
			is.read(reinterpret_cast<char*>(&t), sizeof(T));

			if (is.switch_endian())
			{
				t = lv::endian_switch(t);
			}
		}

		// primitive_buffer_tag
		template<typename T>
		void	save_impl(BinaryOStream & os, T const & t, primitive_buffer_tag)
		{
			if (!std::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if (os.switch_endian() && value_size != 1)
				{
					save_impl(os, t, range_tag());
				}
				else
				{
					os.write(reinterpret_cast<char const*>(&*std::begin(t)), static_cast<std::streamsize>(
						std::size(t) * value_size));
				}
			}
		}

		template<typename T>
		void	load_impl(BinaryIStream & is, T & t, primitive_buffer_tag)
		{
			if (!std::empty(t))
			{
				size_t const value_size = sizeof(typename boost::range_value<T>::type);

				if (is.switch_endian() && value_size != 1)
				{
					load_impl(is, t, range_tag());
				}
				else
				{
					is.read(reinterpret_cast<char*>(&*std::begin(t)), static_cast<std::streamsize>(
						std::size(t) * sizeof(typename boost::range_value<T>::type)));
				}
			}
		}


		// range_tag
		template<typename T>
		void	save_impl(BinaryOStream & os, T const & t, range_tag)
		{
			for (auto const & item : t)
			{
				os << item;
			}
		}

		template<typename T>
		void	load_impl(BinaryIStream & is, T & t, range_tag)
		{
			for (auto & item : t)
			{
				is >> item;
			}
		}
	}

	template<typename T>
	void	save(BinaryOStream & os, T const & t)
	{
		detail::save_impl(os, t, object_tag_t<T>());
	}

	template<typename T>
	void	load(BinaryIStream & is, T & t)
	{
		typedef object_tag_t<std::decay_t<T> >	tag_type;
		detail::load_impl(is, t, tag_type());
	}

} }

#endif
