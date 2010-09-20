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
#define LV_LUAARCHIVE_VECTOR_HPP

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

} } }

#endif