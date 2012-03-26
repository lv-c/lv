// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_FWD_HPP
#define LV_DATAFLOW_FWD_HPP

#include <boost/function.hpp>

namespace lv { namespace flow {

	template<template<class> class PushPolicy, class Port = std::string, class Compare = std::less<Port> >
	class DataFlow;

	template<class Key = std::string, class OArchive = OArchive>
	class Source;

	template<template<class> class PushPolicy, class Key = std::string, class IArchive = IArchive>
	class Sink;

	template<class Key = std::string, class IArchive = IArchive>
	class Registery;

	template<class T>
	class SyncPush;

	template<class T>
	class AsyncPush;

	template<class T>
	class ThreadedPush;

	typedef boost::function<void(BufferPtr)>	slot_type;
	

} }

#endif 