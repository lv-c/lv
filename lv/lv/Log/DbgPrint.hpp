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


#ifndef LOG_DBG_LOGGER
#define LOG_DBG_LOGGER	LogManager::instance().logger()
#endif

#ifdef _DEBUG
#define DbgPrint(exp)	LOG_DBG_LOGGER(lv::debug) << exp
#else
#define DbgPrint(exp)	((void)0)
#endif

#ifdef LOG_ENABLE_WEAK_PRINT
#ifdef _DEBUG
#define WeakPrint(exp)	LOG_DBG_LOGGER(lv::debug - 1) << exp
#else
#define WeakPrint(exp)	((void)0)
#endif
#endif


#endif