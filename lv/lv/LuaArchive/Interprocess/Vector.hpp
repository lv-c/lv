// *********************************************************************
//  Vector   version:  1.0   ��  date: 2012/06/01
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/interprocess/containers/vector.hpp>

#include <lv/LuaArchive/Tags.hpp>
#include <lv/ContainerAdaptor/Interprocess/Vector.hpp>


namespace lv::lua::archive
{
	template<class T, class Ax>
	struct object_tag<boost::interprocess::vector<T, Ax> >
		: sequence_tag
	{
	};

}