// *********************************************************************
//  DbgPrint   version:  1.0   ¡¤  date: 04/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DBGPRINT_HPP
#define LV_DBGPRINT_HPP


#ifdef _DEBUG
#define DbgPrint(log, exp)	log(lv::debug) << exp
#else
#define DbgPrint(log, exp)	((void)0)
#endif

#ifdef LOG_ENABLE_WEAK_PRINT
#ifdef _DEBUG
#define WeakPrint(log, exp)	log(lv::debug - 1) << exp
#else
#define WeakPrint(log, exp)	((void)0)
#endif
#endif


#endif