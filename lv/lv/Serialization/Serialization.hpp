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
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/array_optimization.hpp>
#include <boost/serialization/array_wrapper.hpp>

#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_archive.hpp>
#include <boost/archive/detail/check.hpp>

#include <boost/mpl/eval_if.hpp>

#include <type_traits>


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
				typedef std::remove_extent_t<T> value_type;

				boost::serialization::collection_size_type count(sizeof(t) / sizeof(value_type));
				ar << count << boost::serialization::make_array(static_cast<value_type const*>(&t[0]), count);
			}

			template<typename T>
			static void load(Archive & ar, T & t)
			{
				typedef std::remove_extent_t<T> value_type;
				
				std::size_t current_count = sizeof(t) / sizeof(value_type);

				boost::serialization::collection_size_type count;
				ar >> count;

				if (static_cast<std::size_t>(count) > current_count)
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
				boost::archive::detail::check_object_versioning<T>();

				unsigned int ver = boost::serialization::version<T>::value;

				ar << boost::archive::version_type(ver);
				boost::serialization::serialize_adl(ar, const_cast<T &>(t), ver);
			}

			template<typename T>
			static void load(Archive & ar, T & t)
			{
				boost::archive::version_type file_ver;
				ar >> file_ver;

				if (file_ver > boost::archive::version_type(boost::serialization::version<T>::value))
				{
					throw boost::archive::archive_exception(boost::archive::archive_exception::unsupported_class_version);
				}

				boost::serialization::serialize_adl(ar, t, file_ver);
			}
		};


		namespace mpl = boost::mpl;

		template<class Archive, typename T>
		struct serializer_type
		{
			typedef typename mpl::eval_if<
				std::is_enum<T>,
				mpl::identity<detail::serialize_enum_type<Archive> >,

				typename mpl::eval_if<
					std::is_array<T>,
					mpl::identity<detail::serialize_array_type<Archive> >,
					mpl::identity<detail::serialize_default<Archive> >
				>
			>::type type;
		};

	}


	template<class Archive, typename T>
	void	save(Archive & ar, T const & t, Overload)
	{
		detail::serializer_type<Archive, T>::type::save(ar, t);
	}

	template<class Archive, typename T>
	void	save_adl(Archive & ar, T const & t)
	{
		save(ar, t, Overload());
	}


	template<class Archive, typename T>
	void	load(Archive & ar, T & t, Overload)
	{
		detail::serializer_type<Archive, T>::type::load(ar, t);
	}

	template<class Archive, typename T>
	void	load_adl(Archive & ar, T & t)
	{
		load(ar, t, Overload());
	}

} }


namespace boost { namespace serialization {

	template<>
	struct use_array_optimization<lv::OArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<std::is_arithmetic<T>::value> {};
	};

	template<>
	struct use_array_optimization<lv::IArchive>
	{
		template<class T>
		struct apply : boost::mpl::bool_<std::is_arithmetic<T>::value> {};
	};

} }



#endif
