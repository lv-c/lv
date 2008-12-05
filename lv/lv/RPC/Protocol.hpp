// *********************************************************************
//  Protocol   version:  1.0   ¡¤  date: 09/22/2008
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

namespace lv { namespace rpc {



	struct Protocol
	{
		typedef uint32	request_id_type;

		typedef int32	id_key_type;
		typedef int16	except_key_type;

		/**
		 * purpose of this packet.
		 */
		struct header
		{
			// If you are going to define your own protocol, you can choose other types than int8 type
			//	and they don't have to be integral types.
			// The requirement is that it should be serializable and equality comparable.
			// The requirements are the same for options::type and except::type, but they don't have to 
			// be same type.
			typedef int8 type;

			static type const	confer	= 0x30;
			static type const	call	= 0x31;
			static type const	reply	= 0x32;
		};


		/** 		
		 * options of a call
		 */
		struct options
		{
			typedef int8 type;

			static type const	ret		= 0x40;	// send back the result
			static type const	ack		= 0x41;	// send back an acknowledgment
			static type const	none	= 0x42;	// nothing should be sent back
		};


		/**
		 * whether a reply contains an exception.
		 */
		struct except
		{
			typedef int8 type;

			static type const	has_ex	= 0x50;		// contains an exception
			static type const	no_ex	= 0x51;		// contains no exception
		};
		

	};


} }

#endif // LV_PRC_PROTOCOL_HPP