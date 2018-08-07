// *********************************************************************
//  OArchive   version:  1.0   ¡¤  date: 04/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


#include <lv/Serialization/Serialization.hpp>
#include <lv/Stream/OStreamProxy.hpp>

#include <boost/noncopyable.hpp>
#include <boost/serialization/array_wrapper.hpp>

#include <string>


namespace lv
{
	class OArchive : boost::noncopyable
	{
		OStreamProxy	ostream_;

	public:

		using is_saving = boost::mpl::true_;
		using is_loading = boost::mpl::false_;


		explicit OArchive(std::ostream & os)
			: ostream_(os)
		{
		}

		explicit OArchive(OBufferStream & os)
			: ostream_(os)
		{
		}

		template<class T>
		OArchive & operator << (T const & val)
		{
			save(val);
			return *this;
		}

		template<class T>
		OArchive & operator & (T const & val)
		{
			return *this << val;
		}

		// 
		template<class T>
		void	save_array(boost::serialization::array_wrapper<T> const & a, unsigned int)
		{
			save_binary(a.address(), a.count() * sizeof(T));
		}

		void	save_binary(void const * address, std::size_t count)
		{
			bool success = true;
			try
			{
				ostream_.write(static_cast<char const*>(address), static_cast<std::streamsize>(count));
			}
			catch (std::ios_base::failure const &)
			{
				success = false;
			}

			if (!ostream_ || !success)
			{
				throw boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error);
			}
		}

		unsigned int	get_library_version() const
		{
			return boost::archive::BOOST_ARCHIVE_VERSION();
		}

	protected:

		template<class T>
		void	save(T const & t)
		{
			serialization::save_adl(*this, t);
		}

		void	save(boost::serialization::collection_size_type t)
		{
			save(static_cast<uint32_t>(t));
		}
		
		void	save(boost::archive::version_type ver)
		{
			BOOST_ASSERT(ver < boost::archive::version_type(boost::integer_traits<uint8_t>::const_max));
			save(static_cast<uint8_t>(ver));
		}

		void	save(boost::serialization::item_version_type ver)
		{
			BOOST_ASSERT(ver < boost::serialization::item_version_type(boost::integer_traits<uint8_t>::const_max));
			save(static_cast<uint8_t>(ver));
		}

		void	save(std::string const & str)
		{
			std::size_t size = str.size();
			save(size);
			save_binary(str.data(), size);
		}

		void	save(std::wstring const & wstr)
		{
			std::size_t size = wstr.size();
			save(size);
			save_binary(wstr.data(), size * sizeof(wchar_t) / sizeof(char));
		}

	};



	//
	class OStreamFactory;

	template<class Archive>
	class OArchiveWrapper;

	template<>
	class OArchiveWrapper<OArchive>
	{
		OBufferStream	os_;

		OArchive	ar_;

	public:

		OArchiveWrapper(OStreamFactory &, Buffer & buf)
			: os_(buf)
			, ar_(os_)
		{
		}

		OArchive &	get()
		{
			return ar_;
		}

		void	flush()
		{
			os_.flush();
		}
	};
}
