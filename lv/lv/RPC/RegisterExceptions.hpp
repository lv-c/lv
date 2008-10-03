// *********************************************************************
//  RegisterExceptions   version:  1.0   ¡¤  date: 09/22/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************


// There's no header guard here because this file may be included multiple times

#ifndef RPC_REG_EXCEP
#error "define macro RPC_REG_EXCEP to do what you want."
#define RPC_REG_EXCEP
#endif

// include the user defined exceptions first. Note that the file defined by PRC_USER_EXCEPTIONS
// shouldn't contain header guard. And derived exception class should be registered first than 
// the base exception class in the file PRC_USER_EXCEPTIONS.

#ifdef PRC_USER_EXCEPTIONS
#include PRC_USER_EXCEPTIONS
#endif


// represent all the unregistered exceptions
RPC_REG_EXCEP( RpcUnknownException )

// default exceptions. the user can define RPC_NO_DEFAULT_EXCEPTIONS to disable 
// the registration of all the default exceptions.

#ifndef RPC_NO_DEFAULT_EXCEPTIONS

// logic_error standard subclasses
RPC_REG_EXCEP( std::domain_error )
RPC_REG_EXCEP( std::invalid_argument )
RPC_REG_EXCEP( std::length_error )
RPC_REG_EXCEP( std::out_of_range )
RPC_REG_EXCEP( std::logic_error )

RPC_REG_EXCEP( std::bad_alloc )
RPC_REG_EXCEP( std::bad_cast )
RPC_REG_EXCEP( std::bad_typeid )
RPC_REG_EXCEP( std::bad_exception )


// runtime_error standard subclasses
RPC_REG_EXCEP( std::overflow_error )
RPC_REG_EXCEP( std::range_error )
RPC_REG_EXCEP( std::underflow_error)

RPC_REG_EXCEP( std::runtime_error )



#endif // RPC_NO_DEFAULT_EXCEPTIONS


#undef RPC_REG_EXCEP