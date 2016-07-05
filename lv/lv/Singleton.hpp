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
#ifndef LV_SINGLETON_AHPP
#define LV_SINGLETON_AHPP

#include <boost/assert.hpp>


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
		static	T&	instance()
		{
			return instance_impl<StaticAlloc>();
		}

		static bool	initialized()
		{
			return instance_ != nullptr;
		}

	private:

		// noncopyable
		Singleton( const Singleton& );
		Singleton const & operator = (Singleton const &);


		template <bool StaticAlloc>
		static T & instance_impl()
		{
			static T obj;
			return obj;
		}

		template <>
		static T & instance_impl<false>()
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

	template <class T, bool StaticAlloc>
	T* Singleton<T, StaticAlloc>::instance_ = nullptr;

}

#endif