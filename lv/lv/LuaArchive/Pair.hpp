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

#include <utility>

#include <boost/type_traits/remove_const.hpp>

#include <luabind/object.hpp>

namespace lv { namespace lua { namespace archive {

	namespace detail
	{
		template<typename T>
		typename boost::remove_const<T>::type &	remove_const(T & v)
		{
			return const_cast<boost::remove_const<T>::type &>(v);
		}
	}

	template<typename F, typename S>
	void	save(std::ostream & os, std::pair<F, S> const & v, size_t level)
	{
		detail::save_key_value(os, v.first, v.second, level);
	}

	template<typename F, typename S>
	void	load_item(luabind::iterator const & it, std::pair<F, S> & v)
	{
		// in bimap, F and S are both const
		detail::load_key_value(it, detail::remove_const(v.first), detail::remove_const(v.second));
	}

} } }

#endif