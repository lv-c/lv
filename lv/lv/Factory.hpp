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

#include <vector>
#include <atomic>
#include <functional>
#include <mutex>


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

	// for expensive objects

	template<typename T>
	class Factory : public std::enable_shared_from_this<Factory<T> >
	{
	protected:

		typedef std::lock_guard<std::mutex>	lock_guard;

		std::mutex	mutex_;

		std::vector<T*>	objects_;

		typedef std::function<T*()>	creator_type;
		creator_type	creator_;

		typedef	std::function<void(T*)>	deleter_type;

		static std::atomic<int>	total_num_;

		static std::atomic<int>	current_num_;
	
	public:

		explicit Factory(creator_type const & creator = NewCreator<T>())
			: creator_(creator)
		{
		}

		typedef std::unique_ptr<T, deleter_type>	unique_pointer;

		virtual	unique_pointer	get()
		{
			T *	obj = nullptr;

			{
				lock_guard lock(mutex_);

				if (!objects_.empty())
				{
					obj = objects_.back();
					objects_.pop_back();

					current_num_--;
				}
				else
				{
					obj = creator_();

					total_num_++;
				}

				post_process(*obj);
			}

			return unique_pointer(obj, std::bind(&Factory::release, shared_from_this(), std::placeholders::_1));
		}


		virtual	~Factory()
		{
			lock_guard lock(mutex_);

			for (T * t : objects_)
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
			lock_guard lock(mutex_);

			objects_.push_back(obj);

			current_num_++;
		}
	};

	template<typename T>
	std::atomic<int> Factory<T>::total_num_ = 0;

	template<typename T>
	std::atomic<int> Factory<T>::current_num_ = 0;
}

#endif