// *********************************************************************
//  Singleton   version:  1.0   ·  date: 09/02/2007
//  --------------------------------------------------------------------
//						单件模式的实现
//				由用户负责该唯一对象的创建及销毁
//	如果要在运行时才能判断由谁来创建该对象，可以通过 initialized 
//	函数来查询对象是否已创建，如果还没创建，则创建之。注意这不是线程
//	安全的
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_SINGLETON_AHPP
#define LV_SINGLETON_AHPP

#include <boost/assert.hpp>

namespace lv
{
	template<class T>
	class Singleton
	{
	public:
		static	T&	instance()
		{
			return *instance_;
		}

		static bool	initialized()
		{
			return instance_ != NULL;
		}

	protected:

		Singleton()
		{
			BOOST_ASSERT(instance_ == NULL);
			instance_ = static_cast<T*>(this);
		}
		virtual	~Singleton() 
		{
			instance_ = NULL;
		}

	private:  
		Singleton( const Singleton& );
		Singleton const & operator = (Singleton const &);

	private:

		static	T*	instance_;
	};

	template <class T>
	T* Singleton<T>::instance_ = NULL;

}

#endif // LV_SINGLETON_AHPP