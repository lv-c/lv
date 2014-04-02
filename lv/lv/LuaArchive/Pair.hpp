// *********************************************************************
//  Pair   version:  1.0   ¡¤  date: 09/19/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_PAIR_HPP
#define LV_LUAARCHIVE_PAIR_HPP

#include <utility>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include <luabind/object.hpp>

namespace lv { namespace lua { namespace archive {

	namespace detail
	{
		template<typename T>
		typename boost::remove_const<T>::type &	remove_const(T & v)
		{
			return const_cast<boost::remove_const<T>::type &>(v);
		}
	}

	template<typename F, typename S>
	void	save(std::ostream & os, std::pair<F, S> const & v, size_t level)
	{
		detail::save_key_value(os, v.first, v.second, level);
	}

	template<typename F, typename S>
	void	load_item(luabind::iterator const & it, std::pair<F, S> & v)
	{
		// in bimap, F and S are both const
		detail::load_key_value(it, detail::remove_const(v.first), detail::remove_const(v.second));
	}

	//

	class Parser;

	namespace detail
	{
		template<typename F, typename S>
		void	load_impl(Parser & parser, int index, std::pair<F, S> & v, boost::mpl::true_)
		{
			Token token = parser.next_token();
			parser.rollback(token);

			if(! token.is_key)
			{
				detail::remove_const(v.first) = static_cast<F>(index);
				parser >> detail::remove_const(v.second);
			}
			else
			{
				load_impl(parser, index, v, boost::mpl::false_());
			}
		}

		template<typename F, typename S>
		void	load_impl(Parser & parser, int index, std::pair<F, S> & v, boost::mpl::false_)
		{
			parser >> detail::remove_const(v.first) >> detail::symbol('=') >> detail::remove_const(v.second);
		}
	}

	template<typename F, typename S>
	void	load_item(Parser & parser, int index, std::pair<F, S> & v)
	{
		detail::load_impl(parser, index, v, boost::mpl::bool_<boost::is_arithmetic<F>::value>());
	}

} } }

#endif