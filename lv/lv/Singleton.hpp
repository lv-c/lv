// *********************************************************************
//  Singleton   version:  1.0   ��  date: 09/02/2007
//  --------------------------------------------------------------------
//						����ģʽ��ʵ��
//	�����ֶ��󴴽���ʽ��ѡ��
//	1 �Ǿ�̬������ ���ڵ�һ�ε��� instance ����ʱ�ɸú�������һ����̬��
//		���󡣴˷�ʽ����Ҫ����ʱ֪��Ҫ�����Ķ������͡�
//	2 �Ƕ�̬������ �����û�������ʱ��̬��������Ķ��󡣸ö������Ϳ����� 
//		T �� T �����ࡣ
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/assert.hpp>

#include <type_traits>


namespace lv
{
	/**
	 * @param StaticAlloc if it's false, it's the user's responsibility to 
	 *	create the single object. Otherwise a static object of type T will
	 *	be created when the first time instance() is called.
	 */
	template<class T, bool StaticAlloc = false>
	class Singleton
	{
	public:
		static	T &	instance()
		{
			return instance_impl(std::integral_constant<bool, StaticAlloc>());
		}

		static bool	initialized()
		{
			return instance_ != nullptr;
		}

	private:

		// noncopyable
		Singleton(Singleton const &) = delete;
		Singleton & operator = (Singleton const &) = delete;


		static T & instance_impl(std::true_type)
		{
			static std::remove_const_t<T>	obj;
			return obj;
		}

		static T & instance_impl(std::false_type)
		{
			BOOST_ASSERT(instance_ != nullptr);
			return *instance_;
		}

	protected:

		Singleton()
		{
			BOOST_ASSERT(instance_ == nullptr);
			instance_ = static_cast<T*>(this);
		}

		virtual	~Singleton() 
		{
			instance_ = nullptr;
		}


	private:

		static	T*	instance_;
	};

	template<class T, bool StaticAlloc>
	T* Singleton<T, StaticAlloc>::instance_ = nullptr;

}
