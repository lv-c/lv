// *********************************************************************
//  BinaryObject   version:  1.0   ¡¤  date: 2019/05/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/LuaArchive/Fwd.hpp>

#include <boost/serialization/binary_object.hpp>

#include <iostream>


namespace lv::lua::archive
{
	void	save(std::ostream & os, boost::serialization::binary_object const & t, size_t level);


	void	load(luabind::object const & obj, boost::serialization::binary_object const & t);

	void	load(Parser & parser, boost::serialization::binary_object const & t);
}