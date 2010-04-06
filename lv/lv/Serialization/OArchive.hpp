// *********************************************************************
//  OArchive   version:  1.0   ¡¤  date: 04/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_OARCHIVE_HPP
#define LV_SERIALIZATION_OARCHIVE_HPP


#include <lv/Serialization/Serialization.hpp>
#include <lv/Stream/OStreamProxy.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/serialization/collection_size_type.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_archive.hpp>

#include <string>

namespace lv
{
	class OArchive
	{
		OStreamProxy	ostream_;

	public:

		typedef boost::mpl::true_	is_saving;
		typedef boost::mpl::false_	is_loading;


		explicit OArchive(OStreamProxy const & os)
			: ostream_(os)
		{
		}

		template<typename T>
		OArchive & operator << (T const & val)
		{
			save(val);
			return *this;
		}

		template<typename T>
		OArchive & operator & (T const & val)
		{
			return *this << val;
		}

		// 
		template<typename T>
		void	save_array(boost::serialization::array<T> const & a, unsigned int)
		{
			save_binary(a.address(), a.count() * sizeof(T));
		}

		unsigned int	get_library_version() const
		{
			return boost::archive::BOOST_ARCHIVE_VERSION();
		}

	protected:

		template<typename T>
		typename boost::disable_if<serialization::is_primitive<T> >::type	save(T const & t)
		{
			boost::serialization::serialize(*this, const_cast<T &>(t), 0);
		}

		template<typename T>
		typename boost::enable_if<serialization::is_primitive<T> >::type	save(T t)
		{
			save_binary(&t, sizeof(T));
		}

		void	save(boost::serialization::collection_size_type t)
		{
			save(static_cast<uint32>(t));
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

		void	save_binary(void const * address, std::size_t count)
		{
			ostream_.write(static_cast<char const*>(address), count);

			if(! ostream_.good())
			{
				throw boost::archive::archive_exception(boost::archive::archive_exception::stream_error);
			}
		}

	};
}

#endif