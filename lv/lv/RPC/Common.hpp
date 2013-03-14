// *********************************************************************
//  Common   version:  1.0   ¡¤  date: 12/04/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_COMMON_HPP
#define LV_RPC_COMMON_HPP


#include <lv/Exception.hpp>

#include <boost/serialization/serialization.hpp>

namespace lv { namespace rpc {
	
	DEFINE_EXCEPTION_MSG(InvalidProtocolValue, std::runtime_error);

	// temporary solution
#define	RPC_REGISTER_CLASS(c)	\
	BOOST_CLASS_IMPLEMENTATION(c, boost::serialization::object_serializable)	\
	BOOST_CLASS_TRACKING(c, boost::serialization::track_never)

} }

#endif