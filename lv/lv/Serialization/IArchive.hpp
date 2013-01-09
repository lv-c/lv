// *********************************************************************
//  IArchive   version:  1.0   ¡¤  date: 04/05/2010
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

#include <boost/noncopyable.hpp>

#include <string>

namespace lv
{
	class IArchive : boost::noncopyable
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
		typename boost::disable_if<boost::is_arithmetic<T> >::type	load(T & t)
		{
			serialization::load(*this, t);
		}


		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type	load(T & t)
		{
			load_binary(&t, sizeof(T));
		}

		void	load(boost::serialization::collection_size_type & t)
		{
			uint32	x = 0;
			load(x);

			t = boost::serialization::collection_size_type(x);
		}

		void	load(boost::archive::version_type & ver)
		{
			uint8 x = 0;
			load(x);

			ver = boost::archive::version_type(x);
		}

		void	load(boost::serialization::item_version_type & ver)
		{
			uint8 x = 0;
			load(x);

			ver = boost::serialization::item_version_type(x);
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
			bool success = true;
			try
			{
				istream_.read(static_cast<char *>(address), static_cast<std::streamsize>(count));
			}
			catch(std::ios_base::failure const &)
			{
				success = false;
			}

			if(! istream_.good() || ! success)
			{
				throw boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error);
			}
		}

	};
}

#endif