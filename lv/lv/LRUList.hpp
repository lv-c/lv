// *********************************************************************
//  LRUList   version:  1.0   ¡¤  date: 09/07/2008
//  --------------------------------------------------------------------
//  
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
		typedef std::list<T, Ax>	list_t;

		list_t list_;

	public:

		typedef typename list_t::size_type	size_type;
		typedef typename list_t::value_type	value_type;
		typedef typename list_t::iterator	iterator;
		typedef typename list_t::const_iterator	const_iterator;

		iterator	add(T const & t)
		{
			list_.push_front(t);
			return list_.begin();
		}

		iterator	touch(iterator const & i)
		{
			T value = *i;
			list_.erase(i);
			return add(value);			
		}

		void	erase(iterator const & i)
		{
			list_.erase(i);
		}

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

#endif // LV_LRULIST_HPP