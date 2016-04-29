// *********************************************************************
//  Mapping   version:  1.0   ¡¤  date: 12/29/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_MAPPING_HPP
#define LV_MAPPING_HPP

#include <lv/Exception.hpp>

#include <boost/bimap.hpp>
#include <boost/optional.hpp>

#include <list>

namespace lv
{
	DEFINE_EXCEPTION_MSG(mapping_key_not_found, std::runtime_error)


	template<typename X, typename Y>
	class Mapping
	{
		typedef boost::bimap<X, Y>	bimap_type;
		bimap_type	bimap_;

		// reserve the insert order
		typedef std::list<std::pair<X const, Y const> > list_type;
		list_type	list_;

	public:


		typedef typename list_type::value_type	value_type;
		typedef typename list_type::reference	reference;
		typedef typename list_type::const_reference	const_reference;
		typedef typename list_type::iterator		iterator;
		typedef typename list_type::const_iterator	const_iterator;

		typedef typename bimap_type::left_map	left_map;
		typedef typename bimap_type::right_map	right_map;


		std::pair<iterator, bool> insert(value_type const & v)
		{
			std::pair<left_map::iterator, bool> ret = bimap_.left.insert(v);
			if (ret.second)
			{
				return std::make_pair(list_.insert(list_.end(), v), true);
			}

			BOOST_ASSERT(false);

			return std::make_pair(list_.end(), false);
		}

		/// @exception mapping_key_not_found
		Y	get_left(X const & x) const
		{
			if (boost::optional<Y> o = get_left_optional(x))
			{
				return *o;
			}

			throw mapping_key_not_found();
		}

		/// @exception mapping_key_not_found
		X	get_right(Y const & y) const
		{
			if (boost::optional<X> o = get_right_optional(y))
			{
				return *o;
			}

			throw mapping_key_not_found();
		}

		Y	get_left(X const & x, Y const & default_value) const
		{
			return get_left_optional(x).get_value_or(default_value);
		}


		X	get_right(Y const & y, X const & default_value) const
		{
			return get_right_optional(y).get_value_or(default_value);
		}

		boost::optional<Y>	get_left_optional(X const & x) const
		{
			bimap_type::left_map::const_iterator it = bimap_.left.find(x);
			if (it != bimap_.left.end())
			{
				return it->second;
			}

			return boost::optional<Y>();
		}

		boost::optional<X>	get_right_optional(Y const & y) const
		{
			bimap_type::right_map::const_iterator it = bimap_.right.find(y);
			if (it != bimap_.right.end())
			{
				return it->second;
			}

			return boost::optional<X>();
		}

		left_map const & left() const
		{
			return bimap_.left;
		}

		right_map const & right() const
		{
			return bimap_.right;
		}

		size_t	size() const
		{
			return list_.size();
		}

		bool	empty() const
		{
			return list_.empty();
		}

		void	clear()
		{
			bimap_.clear();
			list_.clear();
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
	};
}


#endif
