// *********************************************************************
//  Config   version:  1.0   ¡¤  date: 09/20/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_CONFIG_HPP
#define LV_RPC_CONFIG_HPP

#ifndef LV_RPC_MAX_ARITY
#	define LV_RPC_MAX_ARITY	8
#endif

// debug support

#ifndef LV_RPC_DEBUG
#	ifdef _DEBUG
#		define LV_RPC_DEBUG	1
#	else
#		define LV_RPC_DEBUG	0
#	endif
#endif





#endif // LV_RPC_CONFIG_HPP