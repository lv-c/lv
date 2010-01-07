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

namespace lv
{
	DEFINE_EXCEPTION_MSG(mapping_key_not_found, std::runtime_error)


	template<typename X, typename Y>
	class Mapping
	{
		typedef boost::bimap<X, Y>	bimap_type;
		bimap_type	bimap_;

	public:

		// sorted by X value

		typedef typename bimap_type::left_map::value_type	value_type;
		typedef typename bimap_type::left_map::iterator		iterator;
		typedef typename bimap_type::left_map::const_iterator	const_iterator;


		std::pair<iterator, bool> insert(value_type const & v)
		{
			return bimap_.left.insert(v);
		}

		/// @exception mapping_key_not_found
		Y	get_left(X const & x)
		{
			if(boost::optional<Y> o = get_left_optional(x))
			{
				return *o;
			}

			throw mapping_key_not_found();
		}

		/// @exception mapping_key_not_found
		X	get_right(Y const & y)
		{
			if(boost::optional<X> o = get_right_optional(y))
			{
				return *o;
			}

			throw mapping_key_not_found();
		}

		Y	get_left(X const & x, Y const & default_value)
		{
			return get_left_optional(x).get_value_or(default_value);
		}


		X	get_right(Y const & y, X const & default_value)
		{
			return get_right_optional(y).get_value_or(default_value);
		}

		boost::optional<Y>	get_left_optional(X const & x)
		{
			bimap_type::left_map::const_iterator it = bimap_.left.find(x);
			if(it != bimap_.left.end())
			{
				return it->second;
			}

			return boost::optional<Y>();
		}

		boost::optional<X>	get_right_optional(Y const & y)
		{
			bimap_type::right_map::const_iterator it = bimap_.right.find(y);
			if(it != bimap_.right.end())
			{
				return it->second;
			}

			return boost::optional<X>();
		}


		size_t	size() const
		{
			return bimap_.size();
		}

		bool	empty() const
		{
			return bimap_.empty();
		}

		iterator	begin()
		{
			return bimap_.left.begin();
		}

		const_iterator	begin() const
		{
			return bimap_.left.begin();
		}

		iterator	end()
		{
			return bimap_.left.end();
		}

		const_iterator	end() const
		{
			return bimap_.left.end();
		}
	};
}


#endif