// *********************************************************************
//  Factory   version:  1.0   ¡¤  date: 04/12/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_FACTORY_HPP
#define LV_FACTORY_HPP


/// hint : define BOOST_SP_USE_QUICK_ALLOCATOR in your program to increase the 
///		performance and make this class more valuable.

#include <lv/Foreach.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <vector>


namespace lv
{

	template<typename T>
	struct NewCreator
	{
		T * operator () () const
		{
			return new T();
		}
	};


	template<typename T>
	class Factory : public boost::enable_shared_from_this<Factory<T> >
	{
	protected:

		typedef boost::mutex::scoped_lock	scoped_lock;
		boost::mutex	mutex_;

		std::vector<T*>	objects_;

		typedef boost::function<T*()>	creator_type;
		creator_type	creator_;

		static volatile long	total_num_;

		static volatile long	current_num_;
	
	public:

		explicit Factory(creator_type const & creator = NewCreator<T>())
			: creator_(creator)
		{
		}

		typedef boost::shared_ptr<T>	shared_pointer;

		virtual	shared_pointer	get()
		{
			scoped_lock lock(mutex_);

			T *	obj = NULL;
			if(! objects_.empty())
			{
				obj = objects_.back();
				objects_.pop_back();

				::InterlockedDecrement(& current_num_);
			}
			else
			{
				obj = creator_();

				::InterlockedIncrement(& total_num_);
			}

			post_process(*obj);

			return shared_pointer(obj, boost::bind(&Factory::release, shared_from_this(), _1));
		}


		virtual	~Factory()
		{
			scoped_lock lock(mutex_);

			foreach(T * t, objects_)
			{
				delete t;
			}

			objects_.clear();
		}


		static	long	current_num()
		{
			return current_num_;
		}

		static	long	total_num()
		{
			return total_num_;
		}

	protected:

		virtual	void	post_process(T & obj)
		{
		}

		virtual void	release(T * obj)
		{
			scoped_lock	lock(mutex_);

			objects_.push_back(obj);

			::InterlockedIncrement(& current_num_);
		}
	};

	template<typename T>
	volatile long Factory<T>::total_num_ = 0;

	template<typename T>
	volatile long Factory<T>::current_num_ = 0;
}

#endif