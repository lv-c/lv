// *********************************************************************
//  StackBalance   version:  1.0   ¡¤  date: 2013/09/17
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

struct lua_State;

namespace lv::lua
{
	class StackBalance
	{
		int	top_;

		lua_State * L_;

	public:

		StackBalance(lua_State * L);

		~StackBalance();
	};

}
