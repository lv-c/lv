// *********************************************************************
//  DbgPrint   version:  1.0   ¡¤  date: 04/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


#ifdef NDEBUG
#	define DbgPrint(log, exp)	((void)0)
#else
#	define DbgPrint(log, exp)	log(lv::debug) << exp
#endif

#ifdef LOG_ENABLE_WEAK_PRINT
#	ifdef NDEBUG
#		define WeakPrint(log, exp)	((void)0)
#	else
#		define WeakPrint(log, exp)	log(lv::debug - 1) << exp
#	endif
#endif
