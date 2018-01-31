// *********************************************************************
//  LRUList   version:  1.0   ¡¤  date: 09/07/2008
//  --------------------------------------------------------------------
//  Least recently used.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <list>

namespace lv
{
	/**
	 * @param Ax allocator type.
	 */
	template<class T, class Ax = std::allocator<T> >
	class LRUList
	{
		using list_type = std::list<T, Ax>;

		list_type list_;

	public:

		using size_type = typename list_type::size_type;
		using value_type = typename list_type::value_type;
		using reference = typename list_type::reference;
		using const_reference = typename list_type::const_reference;
		using iterator = typename list_type::iterator;
		using const_iterator = typename list_type::const_iterator;

		iterator	insert(T const & t)
		{
			list_.push_front(t);
			return list_.begin();
		}

		/**
		 * Brings the item associated with iterator @i to the front of the list.
		 */
		void	touch(iterator const & i)
		{
			list_.splice(list_.begin(), list_, i);
		}

		void	erase(iterator const & i)
		{
			list_.erase(i);
		}

		reference	last()
		{
			return list_.back();
		}

		const_reference	last() const
		{
			return list_.back();
		}

		/**
		 * Erases and returns the LRU item.
		 */
		T		erase_last()
		{
			T value = list_.back();
			list_.pop_back();
			return value;
		}

		void	clear()
		{
			list_.clear();
		}

		size_type	size() const
		{
			return list_.size();
		}

		bool	empty() const
		{
			return list_.empty();
		}

		iterator begin()
		{
			return list_.begin();
		}

		const_iterator begin() const
		{
			return list_.begin();
		}

		iterator end()
		{
			return list_.end();
		}

		const_iterator end() const
		{
			return list_.end();
		}
	};
}