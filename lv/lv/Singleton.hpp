// *********************************************************************
//  Singleton   version:  1.0   ��  date: 09/02/2007
//  --------------------------------------------------------------------
//						����ģʽ��ʵ��
//				���û������Ψһ����Ĵ���������
//	���Ҫ������ʱ�����ж���˭�������ö��󣬿���ͨ�� initialized 
//	��������ѯ�����Ƿ��Ѵ����������û�������򴴽�֮��ע���ⲻ���߳�
//	��ȫ��
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