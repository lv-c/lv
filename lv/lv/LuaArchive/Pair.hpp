// *********************************************************************
//  Pair   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_PAIR_HPP
#define LV_LUAARCHIVE_PAIR_HPP

#include <lv/LuaArchive/ISerializer.hpp>
#include <lv/LuaArchive/OSerializer.hpp>
#include <lv/LuaArchive/PlainISerializer.hpp>

#include <luabind/object.hpp>

#include <utility>
#include <type_traits>


namespace lv::lua::archive
{
	namespace detail
	{
		template<class T>
		std::remove_const_t<T> &	remove_const(T & v)
		{
			return const_cast<std::remove_const_t<T> &>(v);
		}
	}

	template<class F, class S>
	void	save(std::ostream & os, std::pair<F, S> const & v, size_t level)
	{
		detail::save_key_value(os, v.first, v.second, level);
	}

	template<class F, class S>
	void	load_item(luabind::iterator const & it, std::pair<F, S> & v)
	{
		// in bimap, F and S are both const
		detail::load_key_value(it, detail::remove_const(v.first), detail::remove_const(v.second));
	}

	//
	
	namespace detail
	{
		template<class F, class S>
		void	load_impl(Parser & parser, int index, std::pair<F, S> & v, std::false_type)
		{
			parser >> detail::remove_const(v.first) >> detail::symbol('=') >> detail::remove_const(v.second);
		}


		template<class F, class S>
		void	load_impl(Parser & parser, int index, std::pair<F, S> & v, std::true_type)
		{
			Token token = parser.next_token();
			parser.rollback(token);

			if (!token.is_key)
			{
				detail::remove_const(v.first) = static_cast<F>(index);
				parser >> detail::remove_const(v.second);
			}
			else
			{
				load_impl(parser, index, v, std::false_type());
			}
		}
	}

	template<class F, class S>
	void	load_item(Parser & parser, int index, std::pair<F, S> & v)
	{
		detail::load_impl(parser, index, v, std::is_arithmetic<F>());
	}

}

#endif
