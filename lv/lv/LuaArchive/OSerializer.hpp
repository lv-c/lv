// *********************************************************************
//  OSerializer   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_OSERIALIZER_HPP
#define LV_LUAARCHIVE_OSERIALIZER_HPP

#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Common.hpp>
#include <lv/IntType.hpp>
#include <lv/StreamHelper.hpp>

#include <boost/noncopyable.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>


namespace lv { namespace lua { namespace archive {

	namespace detail
	{
		// unknown_tag
		class OArchiveProxy : boost::noncopyable
		{
			std::ostream &	os_;

			size_t	level_;

			bool	primitive_;

			bool	first_time_;

		public:

			typedef boost::mpl::true_	is_saving;
			typedef boost::mpl::false_	is_loading;

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
				if (! primitive_)
				{
					os_ << std::endl << write_tabs(level_ - 1);
				}

				os_ << '}';
			}

			template<typename T>
			OArchiveProxy & operator << (T const & t)
			{
				this->save(t);
				return *this;
			}

			template<typename T>
			OArchiveProxy & operator & (T const & t)
			{
				return *this << t;
			}

		private:

			template<typename T>
			void	save(T const & t)
			{
				if (! first_time_)
				{
					os_ << ", ";
				}

				first_time_ = false;

				if (! is_primitive<T>::value)
				{
					os_ << std::endl << write_tabs(level_);
					primitive_ = false;
				}

				archive::save(os_, t, level_);
			}

		};

		template<typename T>
		void	save_impl(std::ostream & os, T const & t, size_t level, unknown_tag)
		{
			unsigned int version = boost::serialization::version<T>::value;
			OArchiveProxy proxy(os, level + 1, version);
			boost::serialization::serialize(proxy, const_cast<T &>(t), version);
		}

		// primitive_tag
		template<typename T>
		typename boost::enable_if<boost::is_integral<T> >::type	save_primitive(std::ostream & os, T t)
		{
			os << lv::widen_int(t);
		}

		template<typename T>
		typename boost::enable_if<boost::is_float<T> >::type save_primitive(std::ostream & os, T t)
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

		template<typename T>
		void	save_impl(std::ostream & os, T const & t, size_t level, primitive_tag)
		{
			save_primitive(os, t);
		}

		// enum_tag
		template<typename T>
		void	save_impl(std::ostream & os, T t, size_t level, enum_tag)
		{
			archive::save(os, static_cast<int>(t), level);
		}

		// sequence_tag

		template<typename Iter>
		void	save_range(std::ostream & os, Iter begin, Iter end, size_t level)
		{
			os << '{';

			bool is_primitive_item = is_primitive<typename std::iterator_traits<Iter>::value_type>::value;

			for (Iter it = begin; it != end; ++it)
			{
				if (! is_primitive_item)
				{
					os << std::endl << write_tabs(level + 1);
				}

				archive::save(os, *it, level + 1);

				Iter next = it;
				if (++next != end)
				{
					os << ", ";
				}
			}

			if (! is_primitive_item)
			{
				os << std::endl << write_tabs(level);
			}

			os << '}';
		}

		template<typename T>
		void	save_impl(std::ostream & os, T const & t, size_t level, sequence_tag)
		{
			save_range(os, boost::begin(t), boost::end(t), level);
		}

		// utility
		template<typename K, typename V>
		void	save_key_value(std::ostream & os, K const & key, V const & value, size_t level)
		{
			os << '[';
			save(os, key, level);
			os << "] = ";

			save(os, value, level);
		}
	}


	template<typename T>
	void	save(std::ostream & os, T const & t, size_t level)
	{
		detail::save_impl(os, t, level, object_tag<T>::type());
	}

	template<typename T>
	void	save(std::ostream & os, boost::serialization::nvp<T> const & t, size_t level)
	{
		os << t.name() << " = ";
		save(os, t.value(), level);
	}

} } }

#endif