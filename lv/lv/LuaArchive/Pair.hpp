// *********************************************************************
//  Pair   version:  1.0   ��  date: 09/19/2010
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

namespace lv { namespace lua { namespace archive {

	template<typename F, typename S>
	void	save(std::ostream & os, std::pair<F, S> const & v, size_t level)
	{
		os << '[';
		save(os, v.first, level);
		os << "] = ";

		save(os, v.second, level);
	}

	template<typename F, typename S>
	void	load_item(luabind::iterator const & it, std::pair<F, S> & v)
	{
		load(it.key(), v.first);
		load(*it, v.second);
	}

	template<typename F, typename S>
	void	load_item(luabind::iterator const & it, std::pair<F const, S> & v)
	{
		load(it.key(), const_cast<F &>(v.first));
		load(*it, v.second);
	}

} } }

#endif