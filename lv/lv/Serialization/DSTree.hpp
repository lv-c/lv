// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 12/04/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Algorithm/DSTree.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/collection_size_type.hpp>

#include <boost/range/as_array.hpp>


namespace boost::serialization
{
	template<class Key, class Data, class Pred>
	struct version<lv::DSTree<Key, Data, Pred> >
		: mpl::int_<1>
	{
	};


	template<class Archive, class Key, class Data, class Pred>
	void save(Archive & ar, lv::DSTree<Key, Data, Pred> const & tree, unsigned int)
	{
		using tree_type = lv::DSTree<Key, Data, Pred>;

		boost::serialization::collection_size_type const count(tree.size());
		ar << count;

		for (typename tree_type::value_type const & v : tree)
		{
			bool has_data(v.data());
			ar << v.key() << has_data;

			if (has_data)
			{
				ar << (*v.data());
			}
				
			ar << v;
		}
	}

	template<class Archive, class Key, class Data, class Pred>
	void load(Archive & ar, lv::DSTree<Key, Data, Pred> & tree, unsigned int version)
	{
		using tree_type = lv::DSTree<Key, Data, Pred>;

		BOOST_ASSERT(version == 1);		// version 0 has been removed

		tree.clear();

		boost::serialization::collection_size_type count;
		ar >> count;

		tree.reserve(count);

		for (size_t i = 0; i < count; ++i)
		{
			Key key[1];
			typename tree_type::data_pointer data;

			ar >> key[0];

			bool has_data;
			ar >> has_data;

			if (has_data)
			{
				data = std::make_shared<typename tree_type::data_type>();
				ar >> (*data);
			}

			tree.insert(boost::as_array(key), data);

			auto it = tree.find_child(key[0]);
			BOOST_ASSERT(it != tree.end());

			ar >> (*it);
		}
	}

	template<class Archive, class Key, class Data, class Pred>
	void serialize(Archive & ar, lv::DSTree<Key, Data, Pred> & tree, unsigned int version)
	{
		split_free(ar, tree, version);
	}

}
