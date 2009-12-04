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
#include <boost/serialization/optional.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/range/as_array.hpp>

namespace boost { namespace serialization {

	template<typename Archive, typename Ele, typename Info, typename Pred>
	void save(Archive & ar, lv::DSTNode<Ele, Info, Pred> const & tree, unsigned int)
	{
		typedef lv::DSTNode<Ele, Info, Pred> tree_type;

		tree_type::size_type size = tree.size();
		ar & size;

		foreach(tree_type::shared_pointer const & sp, tree)
		{
			tree_type const & p = *sp;

			ar & p.element() & p.info() & p;
		}
	}

	template<typename Archive, typename Ele, typename Info, typename Pred>
	void load(Archive & ar, lv::DSTNode<Ele, Info, Pred> & tree, unsigned int)
	{
		typedef lv::DSTNode<Ele, Info, Pred> tree_type;

		tree_type::size_type size;
		ar & size;

		tree.reserve(size);

		for(tree_type::size_type i = 0; i < size; ++i)
		{
			Ele ele[1];
			boost::optional<Info> info;

			ar & ele[0] & info;
			tree.insert(boost::as_array(ele), info);

			tree_type::iterator it = tree.find_child(ele[0]);
			BOOST_ASSERT(it != tree.end());

			ar & (**it);
		}
	}

	template<typename Archive, typename Ele, typename Info, typename Pred>
	void serialize(Archive & ar, lv::DSTNode<Ele, Info, Pred> & tree, unsigned int version)
	{
		split_free(ar, tree, version);
	}

} }

#endif