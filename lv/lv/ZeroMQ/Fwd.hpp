// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 2012/03/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_ZEROMQ_FWD_HPP
#define LV_ZEROMQ_FWD_HPP

namespace lv { namespace zeromq {

	class BasicSocket;

	template<class Key = std::string, class OArchive = OArchive>
	class SourceSocket;

	template<class Key = std::string, class IArchive = IArchive>
	class SinkSocket;

	class Device;

} }

#endif