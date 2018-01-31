// *********************************************************************
//  SharedPtr   version:  1.0   ¡¤  date: 10/27/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/Fwd.hpp>

#include <memory>


namespace lv::lua::archive
{
	namespace detail
	{
		template<class T>
		void	save_smart_ptr(std::ostream & os, T const & v, size_t level)
		{
			if (v)
			{
				save_adl(os, *v, level);
			}
			else
			{
				os << "nil";
			}
		}

		template<class T, class F>
		void	load_smart_ptr(luabind::object const & obj, T & v, F creator)
		{
			if (luabind::type(obj) == LUA_TNIL)
			{
				v.reset();
			}
			else
			{
				if (!v)
				{
					v = creator();
				}
				load(obj, *v);
			}
		}

		template<class T, class F>
		void	load_smart_ptr(Parser & parser, T & v, F creator)
		{
			if (parser.read_if("nil"))
			{
				v.reset();
			}
			else
			{
				if (!v)
				{
					v = creator();
				}
				parser >> *v;
			}
		}
	}


	template<class T>
	void	save(std::ostream & os, std::shared_ptr<T> const & v, size_t level)
	{
		detail::save_smart_ptr(os, v, level);
	}

	template<class T>
	void	save(std::ostream & os, std::unique_ptr<T> const & v, size_t level)
	{
		detail::save_smart_ptr(os, v, level);
	}

	//
	template<class T>
	void	load(luabind::object const & obj, std::shared_ptr<T> & v)
	{
		detail::load_smart_ptr(obj, v, &std::make_shared<T>);
	}

	template<class T>
	void	load(luabind::object const & obj, std::unique_ptr<T> & v)
	{
		detail::load_smart_ptr(obj, v, &std::make_unique<T>);
	}

	//
	template<class T>
	void	load(Parser & parser, std::shared_ptr<T> & v)
	{
		detail::load_smart_ptr(parser, v, &std::make_shared<T>);
	}

	template<class T>
	void	load(Parser & parser, std::unique_ptr<T> & v)
	{
		detail::load_smart_ptr(parser, v, &std::make_unique<T>);
	}


}
