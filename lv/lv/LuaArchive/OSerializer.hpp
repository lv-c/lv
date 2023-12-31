// *********************************************************************
//  OSerializer   version:  1.0   ��  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Common.hpp>
#include <lv/IntType.hpp>
#include <lv/StreamHelper.hpp>

#include <boost/noncopyable.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

#include <type_traits>


namespace lv::lua::archive
{
	template<class T>
	void	save_adl(std::ostream & os, T const & t, size_t level);


	// unknown_tag
	class OArchiveProxy : boost::noncopyable
	{
		std::ostream &	os_;

		size_t	level_;

		bool	primitive_;

		bool	first_time_;

	public:

		using is_saving = boost::mpl::true_;
		using is_loading = boost::mpl::false_;

		OArchiveProxy(std::ostream & os, size_t level, unsigned int version)
			: os_(os)
			, level_(level)
			, primitive_(true)
			, first_time_(true)
		{
			os_ << '{';
			if (version != DefaultVersion)
			{
				*this << boost::serialization::make_nvp(VersionKey.c_str(), version);
			}
		}

		~OArchiveProxy()
		{
			if (!primitive_)
			{
				os_ << std::endl << write_tabs(level_ - 1);
			}

			os_ << '}';
		}

		template<class T>
		OArchiveProxy & operator << (T const & t)
		{
			this->save(t);
			return *this;
		}

		template<class T>
		OArchiveProxy & operator & (T const & t)
		{
			return *this << t;
		}

	private:

		template<class T>
		void	save(T const & t)
		{
			if (!first_time_)
			{
				os_ << ", ";
			}

			first_time_ = false;

			if (!is_primitive_v<T>)
			{
				os_ << std::endl << write_tabs(level_);
				primitive_ = false;
			}

			archive::save_adl(os_, t, level_);
		}

	};

	namespace detail
	{
		template<class T>
		void	save_impl(std::ostream & os, T const & t, size_t level, unknown_tag)
		{
			unsigned int version = boost::serialization::version<T>::value;
			OArchiveProxy proxy(os, level + 1, version);
			boost::serialization::serialize_adl(proxy, const_cast<T &>(t), version);
		}

		// primitive_tag
		template<class T>
		std::enable_if_t<std::is_integral_v<T> >	save_primitive(std::ostream & os, T t)
		{
			os << lv::widen_int(t);
		}

		template<class T>
		std::enable_if_t<std::is_floating_point_v<T> >	save_primitive(std::ostream & os, T t)
		{
			os << t;
		}

		inline void	save_primitive(std::ostream & os, bool t)
		{
			os << (t ? "true" : "false");
		}

		inline void save_primitive(std::ostream & os, char const * t)
		{
			os << '\'';

			for (; *t != '\0'; ++t)
			{
				switch (*t)
				{
				case '\\':
					os << "\\\\";
					break;

				case '\'':
					os << "\\'";
					break;

				case '\a':
					os << "\\a";
					break;

				case '\b':
					os << "\\b";
					break;

				case '\f':
					os << "\\f";
					break;

				case '\n':
					os << "\\n";
					break;

				case '\r':
					os << "\\r";
					break;

				case '\t':
					os << "\\t";
					break;

				case '\v':
					os << "\\v";
					break;

				default:
					os << *t;
				}
			}

			os << '\'';
		}

		inline void	save_primitive(std::ostream & os, std::string const & t)
		{
			save_primitive(os, t.c_str());
		}

		template<class T>
		void	save_impl(std::ostream & os, T const & t, size_t level, primitive_tag)
		{
			save_primitive(os, t);
		}

		// enum_tag
		template<class T>
		void	save_impl(std::ostream & os, T t, size_t level, enum_tag)
		{
			archive::save_adl(os, static_cast<int>(t), level);
		}

		// sequence_tag

		template<class Iter>
		void	save_range(std::ostream & os, Iter begin, Iter end, size_t level)
		{
			os << '{';

			constexpr bool is_primitive_item = is_primitive_v<typename std::iterator_traits<Iter>::value_type>;

			for (Iter it = begin; it != end; ++it)
			{
				if (!is_primitive_item)
				{
					os << std::endl << write_tabs(level + 1);
				}

				archive::save_adl(os, *it, level + 1);

				Iter next = it;
				if (++next != end)
				{
					os << ", ";
				}
			}

			if (!is_primitive_item)
			{
				os << std::endl << write_tabs(level);
			}

			os << '}';
		}

		template<class T>
		void	save_impl(std::ostream & os, T const & t, size_t level, container_adaptor::container_tag)
		{
			save_range(os, std::begin(t), std::end(t), level);
		}

		// utility
		template<class K, class V>
		void	save_key_value(std::ostream & os, K const & key, V const & value, size_t level)
		{
			os << '[';
			save_adl(os, key, level);
			os << "] = ";

			save_adl(os, value, level);
		}
	}

	BOOST_STRONG_TYPEDEF(size_t, level_type)

	template<class T>
	void	save(std::ostream & os, T const & t, size_t level)
	{
		detail::save_impl(os, t, level, object_tag<T>());
	}

	template<class T>
	void	save_adl(std::ostream & os, T const & t, size_t level)
	{
		level_type lvl(level);
		save(os, t, lvl);
	}

	template<class T>
	void	save(std::ostream & os, boost::serialization::nvp<T> const & t, size_t level)
	{
		os << t.name() << " = ";
		save_adl(os, t.value(), level);
	}

}
