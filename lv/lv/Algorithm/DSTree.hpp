// *********************************************************************
//  DSTree   version:  1.0   ·  date: 09/07/2007
//  --------------------------------------------------------------------
//						Digital Search Tree
//			对孩子结点进行有序插入，然后使用折半查找
//			为提高效率，所以函数均使用非递归实现
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSTREE_HPP
#define LV_DSTREE_HPP

#include <vector>
#include <algorithm>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/range/concepts.hpp>

#include <lv/SharedPtr.hpp>
#include <lv/Foreach.hpp>

namespace lv
{

	template<class Ele, class Info, class Pred = std::less<Ele> >
	class DSTNode
	{
		typedef boost::shared_ptr<DSTNode>	NodePtr;

		Ele		element_;
		Pred	pred_;
		boost::optional<Info>	info_;	// value associated with this node

		typedef std::vector<NodePtr>	children_type;
		children_type	children_;

		DSTNode *	parent_;

		
		static bool node_pred(NodePtr const & lhs, NodePtr const & rhs)
		{
			return lhs->pred_(lhs->element_, rhs->element_);
		}

	private:
		DSTNode(Ele ele, DSTNode * parent, Pred pred = Pred())
			: pred_(pred)
			, element_(ele)
			, parent_(parent)
		{
		}

	public:

		typedef typename children_type::iterator	iterator;
		typedef typename children_type::const_iterator	const_iterator;

		DSTNode(Pred pred = Pred())
			: pred_(pred)
			, parent_(NULL)
		{
		}

		Ele const & element() const
		{
			return element_;
		}

		boost::optional<Info> const & info() const
		{
			return info_;
		}

		boost::optional<Info> & info()
		{
			return info_;
		}

		DSTNode * parent()
		{
			return parent_;
		}

		// boost::ref is supported for all the following sequence
		// that's , seq can be a sequence of reference_wrapper type objects

		// insert a sequence
		template<class RangeT>
		void	insert(RangeT const & seq, Info const & info)
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<RangeT>));	// concept check

			DSTNode * node = this;
			foreach(Ele const & ele, seq)
			{
				iterator it = node->find_child(ele);
				
				if(it == node->end())
				{
					NodePtr new_node(new DSTNode(ele, node, pred_));
					it = node->children_.insert(std::lower_bound(node->begin(), node->end(), new_node,
						&DSTNode::node_pred), new_node);
				}

				node = it->get();
			}

			node->info_ = info;
		}

		template<class RangeT>
		DSTNode * sub_tree(RangeT const & seq)
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<RangeT>));

			DSTNode * node = this;
			foreach(boost::range_value<RangeT>::type const & ele, seq)
			{
				iterator it = node->find_child(ele);
				if(it == node->end())
					return NULL;

				node = it->get();
			}

			return node;
		}


		// find a specified sequence
		template<class RangeT>
		bool	find(RangeT const & seq, Info & info) 		// fixed count
		{
			DSTNode * node = sub_tree(seq);

			if(node == NULL)
				return false;

			if(node->info_)
				info = *node->info_;

			return node->info_;
		}

		/**
		 * Find the first matched sequence
		 * @param size size of the matched sequence
		 */
		template<class RangeT>
		bool first_match(RangeT const & seq, Info & info, size_t * size = NULL) 
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<RangeT>));

			size_t len = 0;

			DSTNode * node = this;
			foreach(Ele const & ele, seq)
			{
				len ++;

				iterator it = node->find_child(ele);
				if(it == node->end())
					return false;
				else if((*it)->info_)
				{
					info = *(*it)->info_;
					if(size != NULL)
						*size = len;

					return true;
				}

				node = it->get();
			}
			
			return false;
		}

		void	clear()
		{
			children_.clear();
		}


		iterator	begin()
		{
			return children_.begin();
		}

		const_iterator begin() const
		{
			return children_.begin();
		}

		iterator end()
		{
			return children_.end();
		}

		const_iterator end() const
		{
			return children_.end();
		}

		bool	empty() const
		{
			return children_.empty();
		}

		size_t	size() const
		{
			return children_.size();
		}

		void erase(iterator it)
		{
			children_.erase(it);
		}

		iterator find_child(Ele const & ele)
		{
			DSTNode node(ele, this, pred_);
			iterator it = std::lower_bound(begin(), end(), lv::shared_from_object(node), &DSTNode::node_pred);
			return (it == end() || pred_(ele, (*it)->element_) ? end() : it);
		}
	};


	// "Today's C++ doesn't support template typedefs."
	template<class Ele, class Info, class Pred = std::less<Ele> >
	struct DSTree 
	{
		typedef DSTNode<Ele, Info, Pred>	type;
	};
}


#endif // LV_DSTREE_HPP