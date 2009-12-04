// *********************************************************************
//  DSTree   version:  1.0   ·  date: 09/07/2007
//  --------------------------------------------------------------------
//						Digital Search Tree
//			对孩子结点进行有序插入，然后使用折半查找
//			为提高效率，所以函数均使用非递归实现
//	TODO: 换一个 iterator, 遍历算法
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
#include <boost/operators.hpp>

#include <lv/SharedPtr.hpp>
#include <lv/Foreach.hpp>

namespace lv
{

	template<class Ele, class Info, class Pred = std::less<Ele> >
	class DSTNode : boost::equality_comparable<DSTNode<Ele, Info, Pred> >
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
		DSTNode(Ele const & ele, DSTNode * parent, Pred pred = Pred())
			: pred_(pred)
			, element_(ele)
			, parent_(parent)
		{
		}

	public:

		typedef typename children_type::iterator	iterator;
		typedef typename children_type::const_iterator	const_iterator;

		typedef typename children_type::size_type	size_type;

		typedef NodePtr	shared_pointer;

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

		DSTNode const * parent() const
		{
			return parent_;
		}

		// boost::ref is supported for all the following sequence
		// that's , seq can be a sequence of reference_wrapper type objects

		// insert a sequence
		// @param info you can use boost::none_t here 
		template<class RangeT>
		void	insert(RangeT const & seq, boost::optional<Info> const & info)
		{
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

		void	reserve(size_type count)
		{
			children_.reserve(count);
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

		size_type	size() const
		{
			return children_.size();
		}

		void erase(iterator it)
		{
			children_.erase(it);
		}

		bool erase(Ele const & ele)
		{
			iterator it = find_child(ele);
			if(it != end())
			{
				erase(it);
				return true;
			}
			else
				return false;
		}

		iterator find_child(Ele const & ele)
		{
			DSTNode node(ele, this, pred_);
			iterator it = std::lower_bound(begin(), end(), lv::shared_from_object(node), &DSTNode::node_pred);
			return (it == end() || pred_(ele, (*it)->element_) ? end() : it);
		}

		bool operator == (DSTNode const & rhs) const
		{
			if(size() != rhs.size())
				return false;

			if(parent_ == NULL)
			{
				if(rhs.parent_ != NULL)
					return false;
			}
			else
			{
				if(element_ != rhs.element_ || info_ != rhs.info_)
					return false;
			}

			for(const_iterator it1 = begin(), it2 = rhs.begin(); it1 != end(); ++it1, ++it2)
			{
				if((**it1) != (**it2))
					return false;
			}

			return true;
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