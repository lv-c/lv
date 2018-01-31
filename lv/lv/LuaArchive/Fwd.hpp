// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

namespace lv
{ 

	class LuaIArchive;
	class LuaOArchive;

	class PlainLuaIArchive;

	namespace lua::archive 
	{
		class Parser;

		class OArchiveProxy;
	}

}

namespace luabind
{
	namespace adl
	{
		class object;
	}

	using adl::object;
}
