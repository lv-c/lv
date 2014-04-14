// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_FWD_HPP
#define LV_LUAARCHIVE_FWD_HPP

namespace lv
{ 

	class LuaIArchive;
	class LuaOArchive;

	class PlainLuaIArchive;

	namespace lua { namespace archive {

		class Parser;

	} }

}

namespace luabind
{
	namespace adl
	{
		class object;
	}

	using adl::object;
}

#endif