// *********************************************************************
//  LRUCache   version:  1.0   ¡¤  date: 08/20/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LRUCACHE_HPP
#define LV_LRUCACHE_HPP

#include <map>

namespace lv
{
	/** 
	 * @param T should be copyable and lightweight
	 */
	template <class Key, class T, class Mutex, class Map = std::map<Key, T> >
	class LRUCache
	{

		typedef Mutex::scoped_lock	scoped_lock;
		Mutex	mutex_;

		Map		map_;

	public:
		
		void	clear()
		{

		}

		size_t	size() const
		{

		}

		bool	empty() const
		{

		}


		bool	fetch(Key const & key, T & value, bool touch = true)
		{

		}

		void	touch(Key const & key)
		{

		}

		void	insert(Key const & key, T const & value)
		{

		}



	};
}


#endif // LV_LRUCACHE_HPP