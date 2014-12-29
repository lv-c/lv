// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 01/21/2011
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2011 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LUAARCHIVE_DSTREE_HPP
#define LV_LUAARCHIVE_DSTREE_HPP

#include <lv/Algorithm/DSTree.hpp>

#include <luabind/object.hpp>

#include <boost/range/as_array.hpp>

#include <deque>

namespace lv { namespace lua { namespace archive {

	namespace detail
	{
		inline void	dstree_save_deque(std::ostream & os, std::deque<char> const & v, size_t level)
		{
			save(os, std::string(v.begin(), v.end()), level);
		}

		template<typename T>
		void	dstree_save_deque(std::ostream & os, std::deque<T> const & v, size_t level)
		{
			save_range(os, v.begin(), v.end(), level);
		}

		template<typename Key, typename Data, typename Pred>
		void	save_dstree_item(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level)
		{
			typedef DSTree<Key, Data, Pred> tree_type;

			std::deque<Key> keys;
			tree_type const * root = &tree;

			while(root->parent() != NULL)
			{
				keys.push_front(root->key());
				root = root->parent();
			}

			os << std::endl << write_tabs(level) << '{';

			bool primitive = true;
			
			if(! is_primitive<Key>::value)
			{
				primitive = false;
				os << std::endl << write_tabs(level + 1);
			}

			dstree_save_deque(os, keys, level + 1);

			os << ", ";

			if(tree.data())
			{
				if(! is_primitive<Data>::value)
				{
					primitive = false;
					os << std::endl << write_tabs(level + 1);
				}

				save(os, *tree.data(), level + 1);
			}
			else
			{
				os << "nil";
			}


			if(! primitive)
			{
				os << std::endl << write_tabs(level);
			}

			os << '}';
		}

		template<typename Key, typename Data, typename Pred>
		void	save_dstree(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level, bool & first_time)
		{
			typedef DSTree<Key, Data, Pred> tree_type;

			BOOST_FOREACH(tree_type::value_type const & v, tree)
			{
				if(v.empty() || v.data())
				{
					if(! first_time)
					{
						os << ", ";
					}

					first_time = false;

					save_dstree_item(os, v, level);
				}

				save_dstree(os, v, level, first_time);
			}
		}


		//

		template<typename Tree, typename Key, typename Data>
		void	dstree_insert(Tree & tree, std::vector<Key> const &, std::string const & str, Data const & data, boost::mpl::true_)
		{
			tree.insert(str, data);
		}

		template<typename Tree, typename Key, typename Data>
		void	dstree_insert(Tree & tree, std::vector<Key> const & keys, std::string const &, Data const & data, boost::mpl::false_)
		{
			tree.insert(keys, data);
		}
	}


	template<typename Key, typename Data, typename Pred>
	void	save(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level)
	{
		typedef DSTree<Key, Data, Pred> tree_type;

		os << '{';

		bool first_time = true;
		detail::save_dstree(os, tree, level + 1, first_time);

		os << std::endl << write_tabs(level) << '}';
	}


	template<typename Key, typename Data, typename Pred>
	void	load(luabind::object const & obj, DSTree<Key, Data, Pred> & tree)
	{
		typedef DSTree<Key, Data, Pred> tree_type;

		tree.clear();

		std::vector<Key> keys;
		std::string str;

		for(luabind::iterator it(obj), end; it != end; ++it)
		{
			luabind::iterator v_it(*it);

			if(boost::is_same<Key, char>::value)
			{
				load(*v_it, str);
			}
			else
			{
				load(*v_it, keys);
			}

			++v_it;

			tree_type::data_pointer data;

			if(v_it != end && luabind::type(*v_it) != LUA_TNIL)
			{
				data.reset(new tree_type::data_type());
				load(*v_it, *data);
			}

			detail::dstree_insert(tree, keys, str, data, boost::is_same<Key, char>::type());
		}
	}

} } }

#endif