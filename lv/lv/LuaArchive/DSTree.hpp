// *********************************************************************
//  DSTree   version:  1.0   ��  date: 01/21/2011
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2011 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Algorithm/DSTree.hpp>

#include <luabind/object.hpp>

#include <boost/range/as_array.hpp>

#include <deque>


namespace lv::lua::archive
{
	namespace detail
	{
		inline void	dstree_save_deque(std::ostream & os, std::deque<char> const & v, size_t level)
		{
			save_adl(os, std::string(v.begin(), v.end()), level);
		}

		template<class T>
		void	dstree_save_deque(std::ostream & os, std::deque<T> const & v, size_t level)
		{
			save_range(os, v.begin(), v.end(), level);
		}

		template<class Key, class Data, class Pred>
		void	save_dstree_item(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level)
		{
			using tree_type = DSTree<Key, Data, Pred>;

			std::deque<Key> keys;
			tree_type const * root = &tree;

			while (root->parent() != nullptr)
			{
				keys.push_front(root->key());
				root = root->parent();
			}

			os << std::endl << write_tabs(level) << '{';

			bool primitive = true;
			
			if (!is_primitive_v<Key>)
			{
				primitive = false;
				os << std::endl << write_tabs(level + 1);
			}

			dstree_save_deque(os, keys, level + 1);

			os << ", ";

			if (tree.data())
			{
				if (!is_primitive_v<Data>)
				{
					primitive = false;
					os << std::endl << write_tabs(level + 1);
				}

				save_adl(os, *tree.data(), level + 1);
			}
			else
			{
				os << "nil";
			}


			if (!primitive)
			{
				os << std::endl << write_tabs(level);
			}

			os << '}';
		}

		template<class Key, class Data, class Pred>
		void	save_dstree(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level, bool & first_time)
		{
			for (auto const & v : tree)
			{
				if (v.empty() || v.data())
				{
					if (!first_time)
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

		template<class Tree, class Key, class Data>
		void	dstree_insert(Tree & tree, std::vector<Key> const &, std::string const & str, Data const & data, std::true_type)
		{
			tree.insert(str, data);
		}

		template<class Tree, class Key, class Data>
		void	dstree_insert(Tree & tree, std::vector<Key> const & keys, std::string const &, Data const & data, std::false_type)
		{
			tree.insert(keys, data);
		}
	}


	template<class Key, class Data, class Pred>
	void	save(std::ostream & os, DSTree<Key, Data, Pred> const & tree, size_t level)
	{
		using tree_type = DSTree<Key, Data, Pred>;

		os << '{';

		bool first_time = true;
		detail::save_dstree(os, tree, level + 1, first_time);

		os << std::endl << write_tabs(level) << '}';
	}


	template<class Key, class Data, class Pred>
	void	load(luabind::object const & obj, DSTree<Key, Data, Pred> & tree)
	{
		using tree_type = DSTree<Key, Data, Pred>;

		tree.clear();

		std::vector<Key> keys;
		std::string str;

		for (luabind::iterator it(obj), end; it != end; ++it)
		{
			luabind::iterator v_it(*it);

			if (std::is_same_v<Key, char>)
			{
				load_adl(*v_it, str);
			}
			else
			{
				load_adl(*v_it, keys);
			}

			++v_it;

			typename tree_type::data_pointer data;

			if (v_it != end && luabind::type(*v_it) != LUA_TNIL)
			{
				data = std::make_shared<typename tree_type::data_type>();
				load_adl(*v_it, *data);
			}

			detail::dstree_insert(tree, keys, str, data, std::is_same<Key, char>());
		}
	}

}
