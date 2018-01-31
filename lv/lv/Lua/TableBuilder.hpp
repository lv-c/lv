// *********************************************************************
//  TableBuilder   version:  1.0   ¡¤  date: 2012/11/11
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/OSerializer.hpp>

#include <sstream>

#include <boost/noncopyable.hpp>


namespace lv::lua
{
	class TableBuilder : boost::noncopyable
	{
		std::ostringstream	oss_;

		bool	first_time_;

	public:

		TableBuilder()
			: first_time_(true)
		{
			oss_ << '{';
		}

		template<class K, class V>
		TableBuilder &	operator () (K const & key, V const & value)
		{
			if (!first_time_)
			{
				oss_ << ", ";
			}

			first_time_ = false;
			
			save_key(key);
			oss_ << " = ";

			lv::lua::archive::save_adl(oss_, value, 0);

			return *this;
		}

		friend std::ostream & operator << (std::ostream & os, TableBuilder const & builder)
		{
			return os << builder.oss_.str() << '}';
		}

	private:

		void	save_key(char const * t)
		{
			oss_ << t;
		}

		void	save_key(std::string const & t)
		{
			oss_ << t;
		}

		template<class T>
		void	save_key(T const & t)
		{
			oss_ << '[';
			lv::lua::archive::save_adl(oss_, t, 0);
			oss_ << ']';
		}

	};

	//

	class ArrayBuilder : boost::noncopyable
	{
		std::ostringstream	oss_;

		bool	first_time_;

	public:

		ArrayBuilder()
			: first_time_(true)
		{
			oss_ << '{';
		}

		template<class T>
		ArrayBuilder & operator , (T const & t)
		{
			if (!first_time_)
			{
				oss_ << ", ";
			}

			first_time_ = false;

			lv::lua::archive::save_adl(oss_, t, 0);

			return *this;
		}

		friend std::ostream & operator << (std::ostream & os, ArrayBuilder const & builder)
		{
			return os << builder.oss_.str() << '}';
		}
	};

	namespace archive
	{
		inline void	save(std::ostream & os, TableBuilder const & t, size_t level)
		{
			os << t;
		}

		inline void	save(std::ostream & os, ArrayBuilder const & t, size_t level)
		{
			os << t;
		}
	}


	//

	template<class T>
	class ObjectBuilder
	{
		T const &	value_;

	public:

		ObjectBuilder(T const & value)
			: value_(value)
		{
		}

		friend std::ostream & operator << (std::ostream & os, ObjectBuilder const & builder)
		{
			lv::lua::archive::save_adl(os, builder.value_, 0);
			return os;
		}
	};

	template<class T>
	ObjectBuilder<T>	make_object_builder(T const & t)
	{
		return ObjectBuilder<T>(t);
	}

}

#define LV_TBL(x, y)	(lv::lua::TableBuilder() (x, y))
#define LV_ARR(...)		(lv::lua::ArrayBuilder(), __VA_ARGS__)

#define LV_OBJ(x)		(lv::lua::make_object_builder(x))
