// *********************************************************************
//  LRUList   version:  1.0   ¡¤  date: 09/07/2008
//  --------------------------------------------------------------------
//  Least recently used.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LRULIST_HPP
#define LV_LRULIST_HPP

#include <list>

namespace lv
{
	/**
	 * @param Ax allocator type.
	 */
	template <typename T, class Ax = std::allocator<T> >
	class LRUList
	{
		typedef std::list<T, Ax>	list_type;

		list_type list_;

	public:

		typedef typename list_type::size_type	size_type;
		typedef typename list_type::value_type	value_type;
		typedef typename list_type::reference	reference;
		typedef typename list_type::const_reference	const_reference;
		typedef typename list_type::iterator	iterator;
		typedef typename list_type::const_iterator	const_iterator;

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

#endif