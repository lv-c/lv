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

		typedef std::vector<NodePtr>	children_t;
		typedef typename children_t::iterator	iterator;
		children_t	children_;

		
		static bool node_pred(NodePtr const & lhs, NodePtr const & rhs)
		{
			return lhs->pred_(lhs->element_, rhs->element_);
		}

	private:
		DSTNode(Ele ele, Pred pred = Pred())
			: pred_(pred)
			, element_(ele)
		{
		}

	public:

		DSTNode(Pred pred = Pred())
			: pred_(pred)
		{
		}

		// insert a sequence
		template<class RangeT>
		void	insert(RangeT const & seq, Info info)
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<RangeT>));	// concept check

			DSTNode * node = this;
			foreach(Ele ele, seq)
			{
				iterator it = node->find_child(ele);
				
				if(it == node->end())
				{
					NodePtr new_node(new DSTNode(ele, pred_));
					it = node->children_.insert(std::lower_bound(node->begin(), node->end(), new_node,
						&DSTNode::node_pred), new_node);
				}

				node = it->get();
			}

			node->info_ = info;
		}

		// find a specified sequence
		template<class RangeT>
		bool	find(RangeT const & seq, Info & info)		// fixed count
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<RangeT>));

			DSTNode* node = this;
			foreach(boost::range_value<RangeT>::type ele, seq)
			{
				iterator it = node->find_child(ele);
				if(it == node->end())
					return false;

				node = it->get();
			}

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
			foreach(Ele ele, seq)
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

	private:

		inline iterator	begin()
		{
			return children_.begin();
		}

		inline iterator end()
		{
			return children_.end();
		}

		inline iterator find_child(Ele ele)
		{
			DSTNode node(ele, pred_);
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