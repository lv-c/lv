// *********************************************************************
//  LRUCache   version:  1.0   ¡¤  date: 12/31/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LRUCACHE_HPP
#define LV_LRUCACHE_HPP

#include <lv/Algorithm/LRUList.hpp>

#include <boost/function.hpp>

#include <map>

namespace lv
{
	template<typename K, typename D, class VAx = std::allocator<std::pair<K const, D> >, 
		class M = std::map<K, LRUList<std::pair<K const, D>, VAx>::iterator> >
	class LRUCache
	{
	public:

		typedef	K	key_type;
		typedef D	data_type;

		typedef pair<key_type const, data_type>	value_type;

		typedef LRUList<value_type, VAx>	list_type;
		typedef M				map_type;

		typedef typename list_type::iterator	iterator;
		typedef typename list_type::const_iterator	const_iterator;
		typedef typename list_type::size_type	size_type;

		typedef boost::function<void(key_type const &, data_type const &)>	removal_callback;

	private:

		list_type	list_;

		map_type	map_;

		size_t		max_size_;

		removal_callback	callback_;

	public:

		LRUCache(size_t max_size, removal_callback callback = removal_callback())
			: max_size_(max_size)
			, callback_(callback)
		{
		}

		size_t	size() const
		{
			return list_.size();
		}

		bool	empty() const
		{
			return list_.empty();
		}

		iterator	begin()
		{
			return list_.begin();
		}

		const_iterator	begin() const
		{
			return list_.begin();
		}

		iterator	end()
		{
			return list_.end();
		}

		const_iterator	end() const
		{
			return list_.end();
		}

		/**
		 * return false if the key already exists
		 */
		pair<iterator, bool>	insert(key_type const & key, data_type const & data)
		{
			map_type::iterator map_it = map_.find(key);

			if(map_it != map_.end())
			{
				return std::make_pair(map_it->second, false);
			}

			list_type::iterator list_it = list_.insert(std::make_pair(key, data));
			map_.insert(std::make_pair(key, list_it));

			if(map_.size() > max_size_ && map_.size() > 1)
			{
				erase(list_.last().first);
			}

			return std::make_pair(list_it, true);
		}
		
		iterator	find(key_type const & key, bool touch = true)
		{
			map_type::iterator map_it = map_.find(key);
			if(map_it != map_.end())
			{
				if(touch)
				{
					list_.touch(map_it->second);
				}

				return map_it->second;
			}

			return end();
		}

		data_type & operator [] (key_type const & key)
		{
			iterator it = find(key);	// find and touch

			if(it == end())
			{
				it = insert(key, data_type()).first;
			}

			return it->second;
		}

		void	touch(key_type const & key)
		{
			map_type::iterator map_it = map_.find(key);
			if(map_it != map_.end())
			{
				list_.touch(it->second);
			}
		}

		void	erase(key_type const & key)
		{
			map_type::iterator map_it = map_.find(key);
			if(map_it != map_.end())
			{
				data_type data = map_it->second->second;

				list_.erase(map_it->second);
				map_.erase(map_it);

				if(callback_)
				{
					callback_(key, data);
				}
			}
		}

	};

}


#endif