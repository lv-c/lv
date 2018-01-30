// *********************************************************************
//  Protocol   version:  1.0   ��  date: 09/22/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_PRC_PROTOCOL_HPP
#define LV_PRC_PROTOCOL_HPP

#include <lv/IntType.hpp>


namespace lv::rpc
{
	struct Protocol
	{
		using request_id_type = uint32;

		/**
		 * purpose of this packet.
		 */
		struct header
		{
			// If you are going to define your own protocol, you can choose other types than int8 type
			//	and they don't have to be integral types.
			// The requirement is that it should be serializable and equality comparable.
			// The requirements are the same for ( options::type and except::type, but they don't have to 
			// be same type.
			using type = int8;

			static type const	call	= 0x30;
			static type const	reply	= 0x31;
		};


		/** 		
		 * options of a call
		 */
		struct options
		{
			using type = int8;

			static type const	ret		= 0x40;	// send back the result
			static type const	ack		= 0x41;	// send back an acknowledgment
			static type const	none	= 0x42;	// nothing should be sent back
		};

	};


}

#endif