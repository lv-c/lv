// *********************************************************************
//  Serialize   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_SERIALIZE_HPP
#define LV_BINARYSTREAM_SERIALIZE_HPP

#include <boost/type_traits/is_pod.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/serialization/serialization.hpp>

#include <lv/Foreach.hpp>

namespace lv { namespace bstream {
	
	// fwd
	template<typename T, class Tag>
	struct Serialize;

	template<typename T, class Enabled>
	struct object_tag;

	template<typename T, class OStream>
	inline void	write(OStream & os, typename boost::call_traits<T>::param_type t)
	{
		Serialize<T, typename object_tag<T>::type>::write(os, t);
	}

	template<typename T, class IStream>
	inline void read(IStream & is, T & t)
	{
		Serialize<T, typename object_tag<T>::type>::read(is, t);
	}


	// tag

#define DEFINE_tag(tag) \
	struct tag : boost::mpl::identity<tag> {}

	DEFINE_tag(pod_tag);
	DEFINE_tag(pod_buffer_tag);	// PODs in continuous memory (vector<PodType>, (w)string ...)
	DEFINE_tag(range_tag);		// list, boost::iterator_range, boost::sub_range ...
	DEFINE_tag(unknown_tag);

#undef DEFINE_tag

	template<typename T, class Enabled = void>
	struct object_tag : unknown_tag {};

	template<typename T>
	struct object_tag<T, typename boost::enable_if<boost::is_pod<T> >::type>
		: pod_tag
	{
	};

	// serialization

	template<typename T>
	struct Serialize<T, unknown_tag>
	{
		template<class OStream>
		static inline void write(OStream & os, T const & t)
		{
			boost::serialization::save(os, t, 1);
		}

		template<class IStream>
		static inline void read(IStream & is, T & t)
		{
			boost::serialization::load(is, t, 1);
		}
	};

	template<typename T>
	struct Serialize<T, pod_tag>
	{
		template<class OStream>
		static inline void write(OStream & os, typename boost::call_traits<T>::param_type t)
		{
			os.write(reinterpret_cast<char const*>(&t), sizeof(T));
		}

		template<class IStream>
		static inline void read(IStream & is, T & t)
		{
			is.read(reinterpret_cast<char*>(&t), sizeof(T));
		}
	};


	template<typename T>
	struct Serialize<T, pod_buffer_tag>
	{
		template<class OStream>
		static void write(OStream & os, T const & t)
		{
			os.write(reinterpret_cast<char const*>(&*boost::begin(t)), boost::size(t) * 
				sizeof(typename boost::range_value<T>::type));
		}

		template<class IStream>
		static void read(IStream & is, T & t)
		{
			is.read(reinterpret_cast<char*>(&*boost::begin(t)), boost::size(t) * 
				sizeof(typename boost::range_value<T>::type));
		}
	};


	template<typename T>
	struct Serialize<T, range_tag>
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
				is >> t;
			}
		}
	};


} }

#endif // LV_BINARYSTREAM_SERIALIZE_HPP