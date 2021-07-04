// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Serialization/Fwd.hpp>
#include <lv/Buffer.hpp>

#include <functional>


namespace lv::flow
{
	template<class Port = std::string, class Compare = std::less<Port> >
	class DataFlow;

	template<class Key = std::string, class OArchive = OArchive>
	class Source;

	template<class Key = std::string, class IArchive = IArchive>
	class Sink;

	template<class Key = std::string, class IArchive = IArchive>
	class Registery;

	using slot_type = std::function<void(BufferPtr)>;

} 
