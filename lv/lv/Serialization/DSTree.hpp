// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 12/04/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_DSTREE_HPP
#define LV_SERIALIZATION_DSTREE_HPP

#include <lv/Algorithm/DSTree.hpp>
#include <lv/Foreach.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/range/as_array.hpp>

namespace boost { namespace serialization {

	template<typename Key, typename Data, typename Pred>
	struct version<lv::DSTree<Key, Data, Pred> >
		: mpl::int_<1>
	{
	};


	template<typename Archive, typename Key, typename Data, typename Pred>
	void save(Archive & ar, lv::DSTree<Key, Data, Pred> const & tree, unsigned int)
	{
		typedef lv::DSTree<Key, Data, Pred> tree_type;

		tree_type::size_type size = tree.size();
		ar & size;

		foreach(tree_type::value_type const & v, tree)
		{
			bool has_data = v.data();
			ar & v.key() & has_data;

			if(has_data)
				ar & (*v.data());
				
			ar & v;
		}
	}

	template<typename Archive, typename Key, typename Data, typename Pred>
	void load(Archive & ar, lv::DSTree<Key, Data, Pred> & tree, unsigned int version)
	{
		typedef lv::DSTree<Key, Data, Pred> tree_type;

		BOOST_ASSERT(version == 1);		// version 0 has been removed

		tree.clear();

		tree_type::size_type size;
		ar & size;

		tree.reserve(size);

		for(tree_type::size_type i = 0; i < size; ++i)
		{
			Key key[1];
			tree_type::data_pointer data;

			ar & key[0];

			bool has_data;
			ar & has_data;

			if(has_data)
			{
				data.reset(new tree_type::data_type());
				ar & (*data);
			}

			tree.insert(boost::as_array(key), data);

			tree_type::iterator it = tree.find_child(key[0]);
			BOOST_ASSERT(it != tree.end());

			ar & (*it);
		}
	}

	template<typename Archive, typename Key, typename Data, typename Pred>
	void serialize(Archive & ar, lv::DSTree<Key, Data, Pred> & tree, unsigned int version)
	{
		split_free(ar, tree, version);
	}

} }

#endif