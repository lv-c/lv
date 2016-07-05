// *********************************************************************
//  Pool   version:  1.0   ¡¤  date: 2015/11/29
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_POOL_HPP
#define LV_POOL_HPP

#include <lv/Ensure.hpp>

#include <boost/pool/singleton_pool.hpp>

#include <memory>


namespace lv { namespace pool {

	struct PoolTag {};

	namespace detail
	{
		template<typename T>
		void	dealloc(T * t)
		{
			t->~T();
			boost::singleton_pool<PoolTag, sizeof(T)>::free(t);
		}
	}

	// use boost::ref if the constructor needs a non-const reference argument
	template<typename T, typename ...Types>
	std::shared_ptr<T>	alloc(Types &&... args)
	{
		typedef boost::singleton_pool<PoolTag, sizeof(T)>	pool_type;

		T * addr = static_cast<T *>(pool_type::malloc());
		LV_ENSURE(addr != nullptr, std::bad_alloc());

		try
		{
			new (addr) T(std::forward<Types>(args)...);
		}
		catch (...)
		{
			pool_type::free(addr);
			throw;
		}

		return std::shared_ptr<T>(addr, &detail::dealloc<T>);
	}

} }

#endif