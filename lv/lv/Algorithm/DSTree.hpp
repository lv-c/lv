// *********************************************************************
//  DSTree   version:  1.0   ¡¤  date: 09/07/2007
//  --------------------------------------------------------------------
//						Digital Search Tree
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/operators.hpp>
#include <boost/range/as_literal.hpp>

#include <memory>

namespace lv
{
	template<class Key, class Data, class Pred = std::less<Key> >
	class DSTree : boost::equality_comparable<DSTree<Key, Data, Pred> >
	{
		using DataPtr = std::shared_ptr<Data>;

		Key			key_;
		DataPtr		data_;

		Pred		pred_;

		using children_type = boost::ptr_vector<DSTree>;
		children_type	children_;

		DSTree	*	parent_;

	private:
		
		DSTree(Key const & key, DSTree * parent, Pred const & pred)
			: key_(key)
			, pred_(pred)
			, parent_(parent)
		{
		}

		struct NodePred
		{
			bool operator () (DSTree const & lhs, Key const & key) const
			{
				return lhs.pred_(lhs.key_, key);
			}

			bool operator () (DSTree const & lhs, DSTree const & rhs) const
			{
				return lhs.pred_(lhs.key_, rhs.key_);
			}
		};

	public:

		using key_type = Key;
		using data_type = Data;

		using value_type = DSTree;

		using iterator = typename children_type::iterator;
		using const_iterator = typename children_type::const_iterator;

		using size_type = typename children_type::size_type;

		using data_pointer = std::shared_ptr<Data>;
		using const_data_pointer = std::shared_ptr<Data const>;


		explicit DSTree(Pred const & pred = Pred())
			: pred_(pred)
			, parent_(nullptr)
		{
		}

		DSTree(DSTree const & other)
		{
			*this = other;	
		}

		DSTree const & operator = (DSTree const & other)
		{
			if (this != &other)
			{
				key_ = other.key_;
				pred_ = other.pred_;

				if (other.data_)
				{
					data_ = std::make_shared<data_type>(*other.data_);
				}
				else
				{
					data_.reset();
				}

				children_ = other.children_;

				for (value_type & v : children_)
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
			for (key_type const & key : seq)
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

		void	insert(char const * seq, data_pointer data)
		{
			insert(boost::as_literal(seq), data);
		}

		void	insert(wchar_t const * seq, data_pointer data)
		{
			insert(boost::as_literal(seq), data);
		}

		template<class KeyRange>
		void	insert(KeyRange const & seq, data_type const & data)
		{
			insert(seq, std::make_shared<data_type>(data));
		}


		template<class KeyRange>
		DSTree * sub_tree(KeyRange const & seq)
		{
			DSTree * tree = this;
			for (auto const & key : seq)
			{
				iterator it = tree->find_child(key);
				if (it == tree->end())
				{
					return nullptr;
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

			if (tree == nullptr)
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
		data_pointer first_match(KeyRange const & seq, size_t * size = nullptr) const
		{
			size_t len = 0;

			DSTree * tree = this;
			for (key_type const & key : seq)
			{
				len ++;

				iterator it = tree->find_child(key);
				if (it == tree->end())
				{
					return data_pointer();
				}
				else if (it->data_)
				{
					if (size != nullptr)
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


		bool operator == (DSTree const & other) const
		{
			if (size() != other.size())
			{
				return false;
			}

			if (parent_ == nullptr)
			{
				if (other.parent_ != nullptr)
				{
					return false;
				}
			}
			else
			{
				if (key_ != other.key_)
				{
					return false;
				}

				 if (data_ && other.data_)
				 {
					 if (*data_ != *other.data_)
					 {
						 return false;
					 }
				 }
				 else
				 {
					 if (bool(data_) != bool(other.data_))
					 {
						 return false;
					 }
				 }
			}

			return std::equal(begin(), end(), other.begin());
		}
	};
}

