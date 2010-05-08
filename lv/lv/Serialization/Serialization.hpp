// *********************************************************************
//  Serialization   version:  1.0   ¡¤  date: 04/06/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_SERIALIZATION_HPP
#define LV_SERIALIZATION_SERIALIZATION_HPP

#include <lv/IntType.hpp>
#include <lv/Serialization/Fwd.hpp>

#include <boost/serialization/collection_size_type.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/serialization.hpp>

#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_archive.hpp>

#include <boost/type_traits/remove_extent.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include <boost/mpl/eval_if.hpp>

namespace lv { namespace serialization {


	namespace detail
	{
		template<class Archive>
		struct serialize_enum_type
		{
			template<typename T>
			static void save(Archive & ar, T t)
			{
				int const i = static_cast<int>(t);
				ar << i;
			}

			template<typename T>
			static void load(Archive & ar, T & t)
			{
				int i;
				ar >> i;
				t = static_cast<T>(i);
			}
		};


		template<class Archive>
		struct serialize_array_type
		{
			template<typename T>
			static void save(Archive & ar, T const & t)
			{
				typedef typename boost::remove_extent<T>::type value_type;

				boost::serialization::collection_size_type count(sizeof(t) / sizeof(value_type));
				ar << count << boost::serialization::make_array(static_cast<value_type const*>(&t[0]), count);
			}

			template<typename T>
			static void load(Archive & ar, T & t)
			{
				typedef typename boost::remove_extent<T>::type value_type;
				
				std::size_t current_count = sizeof(t) / sizeof(value_type);

				boost::serialization::collection_size_type count;
				ar >> count;

				if(static_cast<std::size_t>(count) > current_count)
				{
					throw boost::archive::archive_exception(boost::archive::archive_exception::array_size_too_short);
				}

				ar >> boost::serialization::make_array(static_cast<value_type*>(&t[0]), count);
			}
		};


		template<class Archive>
		struct serialize_default
		{
			template<typename T>
			static void save(Archive & ar, T const & t)
			{
				boost::serialization::serialize(ar, const_cast<T &>(t), 0);
			}

			template<typename T>
			static void load(Archive & ar, T & t)
			{
				boost::serialization::serialize(ar, t, 0);
			}
		};


		namespace mpl = boost::mpl;

		template<class Archive, typename T>
		struct serializer_type
		{
			typedef typename mpl::eval_if<
				boost::is_enum<T>,
				mpl::identity<detail::serialize_enum_type<Archive> >,

				typename mpl::eval_if<
					boost::is_array<T>,
					mpl::identity<detail::serialize_array_type<Archive> >,
					mpl::identity<detail::serialize_default<Archive> >
				>
			>::type type;
		};

	}


	template<class Archive, typename T>
	void	save(Archive & ar, T const & t)
	{
		detail::serializer_type<Archive, T>::type::save(ar, t);
	}

	template<class Archive, typename T>
	void	load(Archive & ar, T & t)
	{
		detail::serializer_type<Archive, T>::type::load(ar, t);
	}

} }


namespace boost { namespace serialization {

	template<>
	struct use_array_optimization<lv::OArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<boost::is_arithmetic<T>::value> {};
	};

	template<>
	struct use_array_optimization<lv::IArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<boost::is_arithmetic<T>::value> {};
	};

} }



#endif