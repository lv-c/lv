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

#include <lv/Config.hpp>
#include <lv/Ensure.hpp>

// some of the following header files include Windows.h and haven't define WIN32_LEAN_AND_MEAN
#ifdef LV_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/pool/singleton_pool.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

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

		// thank boost::optional
		template<typename T, typename Expr>
		void	construct(T * addr, Expr const & expr, void const *)
		{
			new (addr) T(expr);
		}

		template<typename T, typename Expr>
		void	construct(T * addr, Expr const & factory, boost::in_place_factory_base const *)
		{
			factory.template apply<T>(addr);
		}

		template<typename T, typename Expr>
		void	construct(T * addr, Expr const & factory, boost::typed_in_place_factory_base const *)
		{
			factory.apply(addr);
		}
	}

	// use boost::ref if the constructor needs a non-const reference argument
	template<typename T, typename Expr>
	std::shared_ptr<T>	alloc(Expr const & expr)
	{
		typedef boost::singleton_pool<PoolTag, sizeof(T)>	pool_type;

		T * addr = static_cast<T *>(pool_type::malloc());
		LV_ENSURE(addr != 0, std::bad_alloc());

		try
		{
			detail::construct(addr, expr, boost::addressof(expr));
		}
		catch (...)
		{
			pool_type::free(addr);
			throw;
		}

		return std::shared_ptr<T>(addr, &detail::dealloc<T>);
	}

	template<typename T>
	std::shared_ptr<T>	alloc()
	{
		return alloc<T>(boost::in_place_factory0());
	}

} }

#endif