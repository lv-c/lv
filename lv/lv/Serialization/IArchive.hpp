// *********************************************************************
//  IArchive   version:  1.0   ¡¤  date: 04/05/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Serialization/Serialization.hpp>
#include <lv/Stream/IStreamProxy.hpp>

#include <boost/noncopyable.hpp>
#include <boost/serialization/array_wrapper.hpp>
#include <boost/archive/detail/check.hpp>

#include <string>


namespace lv
{
	class IArchive : boost::noncopyable
	{
		IStreamProxy	istream_;

	public:

		using is_loading = boost::mpl::true_;
		using is_saving = boost::mpl::false_;

		explicit IArchive(std::istream & is)
			: istream_(is)
		{
		}

		explicit IArchive(IBufferStream & is)
			: istream_(is)
		{
		}

		template<class T>
		IArchive & operator >> (T & t)
		{
			boost::archive::detail::check_const_loading<T>();
			load(const_cast<std::remove_const_t<T> &>(t));
			return *this;
		}

		template<class T>
		IArchive & operator & (T & t)
		{
			return *this >> t;
		}

		template<class T>
		void	load_array(boost::serialization::array_wrapper<T> & a, unsigned int)
		{
			load_binary(a.address(), a.count() * sizeof(T));
		}

		void	load_binary(void * address, std::size_t count)
		{
			bool success = true;
			try
			{
				istream_.read(static_cast<char *>(address), static_cast<std::streamsize>(count));
			}
			catch (std::ios_base::failure const &)
			{
				success = false;
			}

			if (!istream_ || !success)
			{
				throw boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error);
			}
		}

		unsigned int	get_library_version() const
		{
			return boost::archive::BOOST_ARCHIVE_VERSION();
		}

		// What?
		void	reset_object_address(const void * new_address, const void * old_address)
		{
		}

	protected:


		template<class T>
		void	load(T & t)
		{
			serialization::load_adl(*this, t);
		}

		void	load(boost::serialization::collection_size_type & t)
		{
			uint32_t x = 0;
			load(x);

			t = boost::serialization::collection_size_type(x);
		}

		void	load(boost::archive::version_type & ver)
		{
			uint8_t x = 0;
			load(x);

			ver = boost::archive::version_type(x);
		}

		void	load(boost::serialization::item_version_type & ver)
		{
			uint8_t x = 0;
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

	};


	//
	class IStreamFactory;

	template<class Archive>
	class IArchiveWrapper;

	template<>
	class IArchiveWrapper<IArchive>
	{
		IBufferStream	is_;

		IArchive	ar_;

	public:

		IArchiveWrapper(IStreamFactory &, ConstBufferRef const & buf)
			: is_(buf)
			, ar_(is_)
		{
		}

		IArchive &	get()
		{
			return ar_;
		}
	};
}
