// *********************************************************************
//  IArchive   version:  1.0   ��  date: 04/05/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_IARCHIVE_HPP
#define LV_SERIALIZATION_IARCHIVE_HPP

#include <lv/Serialization/Serialization.hpp>
#include <lv/Stream/IStreamProxy.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/serialization/collection_size_type.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_archive.hpp>

#include <string>

namespace lv
{
	class IArchive
	{
		IStreamProxy	istream_;

	public:

		typedef boost::mpl::true_	is_loading;
		typedef boost::mpl::false_	is_saving;


		explicit IArchive(IStreamProxy const & is)
			: istream_(is)
		{
		}

		template<typename T>
		IArchive & operator >> (T & t)
		{
			load(const_cast<typename boost::remove_const<T>::type &>(t));
			return *this;
		}

		template<typename T>
		IArchive & operator & (T & t)
		{
			return *this >> t;
		}

		template<typename T>
		void	load_array(boost::serialization::array<T> & a, unsigned int)
		{
			load_binary(a.address(), a.count() * sizeof(T));
		}

		unsigned int	get_library_version() const
		{
			return boost::archive::BOOST_ARCHIVE_VERSION();
		}

		// What?
		void reset_object_address(const void * new_address, const void * old_address)
		{
		}

	protected:


		template<typename T>
		typename boost::disable_if<serialization::is_primitive<T> >::type	load(T & t)
		{
			boost::serialization::serialize(*this, t, 0);
		}


		template<typename T>
		typename boost::enable_if<serialization::is_primitive<T> >::type	load(T & t)
		{
			load_binary(&t, sizeof(T));
		}

		void	load(boost::serialization::collection_size_type & t)
		{
			uint32	x = 0;
			load(x);

			t = boost::serialization::collection_size_type(x);
		}

		void	load(std::string & str)
		{
			std::size_t size;
			load(size);

			str.resize(size);
			load_binary(const_cast<char *>(str.data()), size);
		}

		void	load(std::wstring & wstr)
		{
			std::size_t size;
			load(size);

			wstr.resize(size);
			load_binary(const_cast<wchar_t *>(wstr.data()), size * sizeof(wchar_t) / sizeof(char));
		}



		void	load_binary(void * address, std::size_t count)
		{
			istream_.read(static_cast<char *>(address), count);

			if(! istream_.good())
			{
				throw boost::archive::archive_exception(boost::archive::archive_exception::stream_error);
			}
		}

	};
}

#endif