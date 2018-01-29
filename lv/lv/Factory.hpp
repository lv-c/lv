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

		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	mutex_;

		std::vector<T*>	objects_;

		using creator_type = std::function<T*()>;
		creator_type	creator_;

		using deleter_type = std::function<void(T*)>;

	public:

		explicit Factory(creator_type const & creator = NewCreator<T>())
			: creator_(creator)
		{
		}

		using unique_pointer = std::unique_ptr<T, deleter_type>;

		virtual	unique_pointer	get()
		{
			T *	obj = nullptr;

			{
				lock_guard lock(mutex_);

				if (!objects_.empty())
				{
					obj = objects_.back();
					objects_.pop_back();
				}
				else
				{
					obj = creator_();
				}

				post_process(*obj);
			}

			return unique_pointer(obj, std::bind(&Factory<T>::release, this->shared_from_this(), std::placeholders::_1));
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

	protected:

		virtual	void	post_process(T & obj)
		{
		}

		virtual void	release(T * obj)
		{
			lock_guard lock(mutex_);

			objects_.push_back(obj);
		}
	};

}

#endif
