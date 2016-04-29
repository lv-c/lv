// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 09/07/2007
//  --------------------------------------------------------------------
//						Digital Search Tree
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DSTREE_HPP
#define LV_DSTREE_HPP

#include <lv/Foreach.hpp>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/operators.hpp>

#include <functional>

namespace lv
{
	template<class Key, class Data, class Pred = std::less<Key> >
	class DSTree : boost::equality_comparable<DSTree<Key, Data, Pred> >
	{
		typedef boost::shared_ptr<Data>	DataPtr;

		Key			key_;
		DataPtr		data_;

		Pred		pred_;

		typedef boost::ptr_vector<DSTree>	children_type;
		children_type	children_;

		DSTree	*	parent_;

	private:
		
		DSTree(Key const & key, DSTree * parent, Pred const & pred)
			: pred_(pred)
			, key_(key)
			, parent_(parent)
		{
		}

		struct NodePred
		{
			bool operator () (DSTree const & lhs, Key const & key) const
			{
				return lhs.pred_(lhs.key_, key);
			}

			// Bug of vc2005?
			bool operator () (Key const & key, DSTree const & rhs) const
			{
				return rhs.pred_(key, rhs.key_);
			}

			bool operator () (DSTree const & lhs, DSTree const & rhs) const
			{
				return lhs.pred_(lhs.key_, rhs.key_);
			}
		};

	public:

		typedef typename Key	key_type;
		typedef typename Data	data_type;

		typedef DSTree	value_type;

		typedef typename children_type::iterator	iterator;
		typedef typename children_type::const_iterator	const_iterator;

		typedef typename children_type::size_type	size_type;

		typedef boost::shared_ptr<Data>			data_pointer;
		typedef boost::shared_ptr<Data const>	const_data_pointer;


		explicit DSTree(Pred const & pred = Pred())
			: pred_(pred)
			, parent_(NULL)
		{
		}

		DSTree(DSTree const & rhs)
		{
			*this = rhs;	
		}

		DSTree const & operator = (DSTree const & rhs)
		{
			if (this != &rhs)
			{
				key_ = rhs.key_;
				pred_ = rhs.pred_;

				if (rhs.data_)
				{
					data_.reset(new data_type(*rhs.data_));
				}
				else
				{
					data_.reset();
				}

				children_ = rhs.children_;

				BOOST_FOREACH(value_type & v, children_)
				{
					v.parent_ = this;
				}
			}

			return *this;
		}


		key_type const & key() const
		{
			return key_;
		}

		data_pointer	data()
		{
			return data_;
		}

		const_data_pointer	data() const
		{
			return data_;
		}

		DSTree *	parent()
		{
			return parent_;
		}

		DSTree const * parent() const
		{
			return parent_;
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

		bool erase(key_type const & key)
		{
			iterator it = find_child(key);
			if (it != end())
			{
				erase(it);
				return true;
			}
			else
			{
				return false;
			}
		}

		template<class KeyRange>
		void	insert(KeyRange const & seq, data_pointer data)
		{
			DSTree * tree = this;
			BOOST_FOREACH(key_type const & key, seq)
			{
				iterator it = tree->find_child(key);

				if (it == tree->end())
				{
					DSTree * new_node = new DSTree(key, tree, pred_);
					it = tree->children_.insert(std::lower_bound(tree->begin(), tree->end(), key,
						NodePred()), new_node);
				}

				tree = &*it;
			}

			tree->data_ = data;
		}

		template<class KeyRange>
		void	insert(KeyRange const & seq, data_type const & data)
		{
			insert(seq, data_pointer(new data_type(data)));
		}


		template<class KeyRange>
		DSTree * sub_tree(KeyRange const & seq)
		{
			DSTree * tree = this;
			BOOST_FOREACH(boost::range_value<KeyRange>::type const & key, seq)
			{
				iterator it = tree->find_child(key);
				if (it == tree->end())
				{
					return NULL;
				}

				tree = &*it;
			}

			return tree;
		}


		// find a specified sequence
		template<class KeyRange>
		data_pointer	find(KeyRange const & seq) 		// fixed count
		{
			DSTree * tree = sub_tree(seq);

			if (tree == NULL)
			{
				return data_pointer();
			}

			return tree->data_;
		}

		/**
		 * Find the first matched sequence
		 * @param size size of the matched sequence
		 */
		template<class KeyRange>
		data_pointer first_match(KeyRange const & seq, size_t * size = NULL) const
		{
			size_t len = 0;

			DSTree * tree = this;
			BOOST_FOREACH(key_type const & key, seq)
			{
				len ++;

				iterator it = tree->find_child(key);
				if (it == tree->end())
				{
					return data_pointer();
				}
				else if (it->data_)
				{
					if (size != NULL)
					{
						*size = len;
					}

					return it->data_;
				}

				tree = &*it;
			}

			return data_pointer();
		}

		iterator find_child(key_type const & key)
		{
			iterator it = std::lower_bound(begin(), end(), key, NodePred());
			return (it == end() || pred_(key, it->key_) ? end() : it);
		}


		bool operator == (DSTree const & rhs) const
		{
			if (size() != rhs.size())
			{
				return false;
			}

			if (parent_ == NULL)
			{
				if (rhs.parent_ != NULL)
				{
					return false;
				}
			}
			else
			{
				if (key_ != rhs.key_)
				{
					return false;
				}

				 if (data_ && rhs.data_)
				 {
					 if (*data_ != *rhs.data_)
					 {
						 return false;
					 }
				 }
				 else
				 {
					 if (bool(data_) != bool(rhs.data_))
					 {
						 return false;
					 }
				 }
			}

			return std::equal(begin(), end(), rhs.begin());
		}
	};
}


#endif