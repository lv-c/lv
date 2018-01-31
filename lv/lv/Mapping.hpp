// *********************************************************************
//  Mapping   version:  1.0   ¡¤  date: 12/29/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>

#include <boost/bimap.hpp>

#include <list>
#include <optional>


namespace lv
{
	DEFINE_EXCEPTION_MSG(mapping_key_not_found, std::runtime_error)

	DEFINE_EXCEPTION_MSG(dunplicated_key, std::runtime_error)


	template<class X, class Y>
	class Mapping
	{
		using bimap_type = boost::bimap<X, Y>;
		bimap_type	bimap_;

		// reserve the insert order
		using list_type = std::list<std::pair<X const, Y const> >;
		list_type	list_;

	public:


		using value_type = typename list_type::value_type;
		using reference = typename list_type::reference;
		using const_reference = typename list_type::const_reference;
		using iterator = typename list_type::iterator;
		using const_iterator = typename list_type::const_iterator;

		using left_map = typename bimap_type::left_map;
		using right_map = typename bimap_type::right_map;


		Mapping() = default;

		Mapping(std::initializer_list<value_type> const & lst)
		{
			for (value_type const & v : lst)
			{
				insert(v);
			}
		}


		/// @exception dunplicated_key
		void	insert(value_type const & v)
		{
			auto ret = bimap_.left.insert(v);
			LV_ENSURE(ret.second, dunplicated_key());

			list_.insert(list_.end(), v);
		}

		/// @exception mapping_key_not_found
		Y	get_left(X const & x) const
		{
			return get_impl(bimap_.left, x);
		}

		/// @exception mapping_key_not_found
		X	get_right(Y const & y) const
		{
			return get_impl(bimap_.right, y);
		}

		Y	get_left(X const & x, Y const & default_value) const
		{
			return get_impl(bimap_.left, x, default_value);
		}


		X	get_right(Y const & y, X const & default_value) const
		{
			return get_impl(bimap_.right, y, default_value);
		}

		std::optional<Y>	get_left_optional(X const & x) const
		{
			return get_optional_impl(bimap_.left, x);
		}

		std::optional<X>	get_right_optional(Y const & y) const
		{
			return get_optional_impl(bimap_.right, y);
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

	private:

		template<class M, class K>
		decltype(auto)	get_impl(M const & m, K const & k) const
		{
			auto it = m.find(k);
			LV_ENSURE(it != m.end(), mapping_key_not_found());

			return it->second;
		}

		template<class M, class K, class V>
		decltype(auto)	get_impl(M const & m, K const & k, V const & v) const
		{
			auto it = m.find(k);
			return (it == m.end() ? v : it->second);
		}

		template<class M, class K>
		std::optional<std::remove_const_t<typename M::mapped_type> >	get_optional_impl(M const & m, K const & k) const
		{
			auto it = m.find(k);

			if (it == m.end())
			{
				return std::nullopt;
			}
			else
			{
				return it->second;
			}
		}

	};
}

