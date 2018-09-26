// *********************************************************************
//  LRUCache   version:  1.0   ¡¤  date: 12/31/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Algorithm/LRUList.hpp>

#include <functional>
#include <map>
#include <chrono>
#include <limits>


namespace lv
{
	template<class K, class D>
	class LRUCache
	{
	public:

		using key_type = K;
		using data_type = D;

		using clock_type = std::chrono::steady_clock;
		using time_point = clock_type::time_point;
		using duration = clock_type::duration;


		struct value_type
		{
			key_type const	key;

			data_type		data;

			time_point		active_time;
		};


		using list_type = LRUList<value_type>;
		using map_type = std::map<key_type, typename list_type::iterator>;

		using iterator = typename list_type::iterator;
		using const_iterator = typename list_type::const_iterator;
		using size_type = typename list_type::size_type;

		using removal_callback = std::function<void(key_type const &, data_type const &)>;

	private:

		list_type	list_;

		map_type	map_;

		size_t		max_size_;

		duration	timeout_;

		removal_callback	callback_;

	public:

		explicit LRUCache(removal_callback callback = removal_callback())
			: max_size_(std::numeric_limits<size_t>::max())
			, timeout_(duration::max())
			, callback_(std::move(callback))
		{
		}

		void	set_max_size(size_t max_size)
		{
			this->max_size_ = max_size;
		}

		void	set_timeout(duration timeout)
		{
			this->timeout_ = timeout;
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

		// return false if the key already exists or max_size_ == 0
		std::pair<iterator, bool>	insert(key_type const & key, data_type data)
		{
			map_type::iterator map_it = map_.find(key);

			if (map_it != map_.end())
			{
				return { map_it->second, false };
			}

			if (max_size_ == 0)
			{
				list_.clear();
				map_.clear();

				return { end(), false };
			}

			check_impl(max_size_ - 1);

			list_type::iterator list_it = list_.insert({ key, std::move(data), clock_type::now() });
			map_.emplace(key, list_it);

			return { list_it, true };
		}

		void	check_timeout()
		{
			check_impl(max_size_);
		}
		
		iterator	find(key_type const & key, bool touch = true)
		{
			map_type::iterator map_it = map_.find(key);
			if (map_it != map_.end())
			{
				if (touch)
				{
					touch_impl(map_it->second);
				}

				return map_it->second;
			}

			return end();
		}

		data_type & operator [] (key_type const & key)
		{
			iterator it = find(key);	// find and touch

			if (it == end())
			{
				it = insert(key, data_type()).first;
			}

			return it->data;
		}

		void	touch(key_type const & key)
		{
			map_type::iterator map_it = map_.find(key);
			if (map_it != map_.end())
			{
				touch_impl(it->second);
			}
		}

		void	erase(key_type const & key)
		{
			map_type::iterator map_it = map_.find(key);
			if (map_it != map_.end())
			{
				data_type data = std::move(map_it->second->data);

				list_.erase(map_it->second);
				map_.erase(map_it);

				if (callback_)
				{
					callback_(key, data);
				}
			}
		}

	private:

		void	check_impl(size_t max_size)
		{
			time_point now = clock_type::now();

			while (!list_.empty())
			{
				if (list_.size() > max_size || now - list_.last().active_time > timeout_)
				{
					erase(list_.last().key);
				}
				else
				{
					break;
				}
			}
		}

		void	touch_impl(iterator it)
		{
			it->active_time = clock_type::now();
			list_.touch(it);
		}

	};

}
