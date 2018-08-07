// *********************************************************************
//  Buffer   version:  1.0   ��  date: 11/18/2007
//  --------------------------------------------------------------------
//		�� vector<char> ��Ϊ Buffer ��
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Ensure.hpp>

#include <boost/assert.hpp>

#include <vector>
#include <memory>
#include <type_traits>


namespace lv
{
	using Buffer = std::vector<char>;
	using BufferPtr = std::shared_ptr<Buffer>;
	using ConstBufferPtr = std::shared_ptr<Buffer const>;


	/// these two classes are compatible with boost.range and boost.BOOST_FOREACH
	template<class T> class ConstBufferRefT;
	template<class T> class BufferRefT;

	using ConstBufferRef = ConstBufferRefT<char>;
	using BufferRef = BufferRefT<char>;


	template<class T>
	class BufferRefT
	{
		T *		data_;
		size_t	size_;

		template<class> friend class ConstBufferRefT;

		std::shared_ptr<std::vector<T> >	holder_;

	public:

		using reference = T &;
		using const_reference = T const &;

		using iterator = T *;
		using const_iterator = T const *;

		BufferRefT()
			: data_(nullptr)
			, size_(0)
		{
		}

		BufferRefT(T * data, size_t size)
			: data_(data)
			, size_(size)
		{
		}

		template<size_t N>
		BufferRefT(T (&buf)[N])
			: data_(buf)
			, size_(N)
		{
		}

		BufferRefT(std::vector<T> & buf)
			: data_(buf.empty() ? nullptr : &buf[0])
			, size_(buf.size())
		{
		}

		BufferRefT(std::shared_ptr<std::vector<T> > & buf_ptr)
			: data_(buf_ptr->empty() ? nullptr : &(*buf_ptr)[0])
			, size_(buf_ptr->size())
			, holder_(buf_ptr)
		{
		}


		T * data()
		{
			return data_;
		}

		T const * data() const
		{
			return data_;
		}

		size_t	size() const
		{
			return size_;
		}

		bool empty() const
		{
			return data_ == nullptr || size_ == 0;
		}

		iterator begin()
		{
			return data_;
		}
		const_iterator begin() const
		{
			return data_;
		}

		iterator end()
		{
			return data_ + size_;
		}

		const_iterator end() const
		{
			return data_ + size_;
		}

		reference operator [] (size_t index)
		{
			BOOST_ASSERT(data_ != nullptr);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}

		const_reference operator[] (size_t index) const
		{
			BOOST_ASSERT(data_ != nullptr);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}


		BufferRefT	sub(size_t off, size_t count = -1)
		{
			LV_ENSURE(off <= size_, std::out_of_range("BufferRefT::sub out of range"));

			BufferRefT ret(data_ + off, std::min(size_ - off, count));
			ret.holder_ = holder_;

			return ret;
		}
	};


	template<class T>
	class ConstBufferRefT
	{
		T const *	data_;
		size_t		size_;

		std::shared_ptr<std::vector<T> >	holder_;

	public:

		using const_reference = T const &;

		using iterator = T const *;
		using const_iterator = T const *;


		ConstBufferRefT()
			: data_(nullptr)
			, size_(0)
		{
		}

		ConstBufferRefT(T const * data, size_t size)
			: data_(data)
			, size_(size)
		{
		}

		template<size_t N>
		ConstBufferRefT(T const (&buf)[N])
			: data_(buf)
			, size_(N)
		{
		}

		ConstBufferRefT(std::vector<T> const & buf)
			: data_(buf.empty() ? nullptr : &buf[0])
			, size_(buf.size())
		{
		}

		ConstBufferRefT(std::shared_ptr<std::vector<T> > const & buf_ptr)
			: data_(buf_ptr->empty() ? nullptr : &(*buf_ptr)[0])
			, size_(buf_ptr->size())
			, holder_(buf_ptr)
		{
		}

		ConstBufferRefT(BufferRefT<T> const & buf)
			: data_(buf.data())
			, size_(buf.size())
			, holder_(buf.holder_)
		{
		}

		T const * data() const
		{
			return data_;
		}

		size_t size() const 
		{
			return size_;
		}

		bool empty() const
		{
			return data_ == nullptr || size_ == 0;
		}

		const_iterator begin() const
		{
			return data_;
		}

		const_iterator end() const
		{
			return data_ + size_;
		}

		const_reference operator[] (size_t index) const
		{
			BOOST_ASSERT(data_ != nullptr);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}


		ConstBufferRefT	sub(size_t off, size_t count = -1)
		{
			LV_ENSURE(off <= size_, std::out_of_range("ConstBufferRefT::sub out of range"));

			ConstBufferRefT ret(data_ + off, std::min(size_ - off, count));
			ret.holder_ = holder_;

			return ret;
		}
	};


	namespace buffer
	{
		// append

		// Write to the end of the buffer . You'd better reserve enough space to prevent frequent
		// memory allocation.
		inline void append(Buffer & buf, void const * data, size_t size)
		{
			buf.insert(buf.end(), static_cast<char const *>(data), static_cast<char const *>(data) + size);
		}

		inline void	append(Buffer & buf, ConstBufferRef const & data)
		{
			buf.insert(buf.end(), data.begin(), data.end());
		}

		template<class T>
		std::enable_if_t<std::is_arithmetic_v<T> >	append(Buffer & buf, T t)
		{
			append(buf, &t, sizeof(t));
		}


		// insert

		inline void	insert(Buffer & buf, size_t pos, void const * data, size_t size)
		{
			if (pos > buf.size())
			{
				throw std::out_of_range("buffer::insert out of range");
			}

			buf.insert(buf.begin() + pos, static_cast<char const *>(data), static_cast<char const *>(data) + size);
		}

		template<class T>
		std::enable_if_t<std::is_arithmetic_v<T> >	insert(Buffer & buf, size_t pos, T t)
		{
			insert(buf, pos, &t, sizeof(t));
		}


		// write

		/// @exception std::out_of_range
		inline	void write(BufferRef buf, size_t pos, void const * data, size_t size)
		{
			if (pos + size > buf.size())
			{
				throw std::out_of_range("buffer::write out of range");
			}

			std::copy(static_cast<char const *>(data), static_cast<char const *>(data) + size, buf.data() + pos);
		}

		template<class T>
		std::enable_if_t<std::is_arithmetic_v<T> >	write(BufferRef buf, size_t pos, T t)
		{
			write(buf, pos, &t, sizeof(t));
		}


		// read
		
		/// @exception std::out_of_range
		inline	void read(ConstBufferRef const & buf, size_t pos, void * data, size_t size)
		{
			if (pos + size > buf.size())
			{
				throw std::out_of_range("buffer::read out of range");
			}

			std::copy(buf.data() + pos, buf.data() + pos + size, static_cast<char *>(data));
		}

		template<class T>
		std::enable_if_t<std::is_arithmetic_v<T> >	read(ConstBufferRef const & buf, size_t pos, T & t)
		{
			read(buf, pos, &t, sizeof(t));
		}

		template<class T>
		T	read(ConstBufferRef const & buf, size_t pos)
		{
			T t;
			read(buf, pos, t);

			return t;
		}

		//
		inline std::string	to_string(ConstBufferRef const & buf)
		{
			return std::string(buf.data(), buf.size());
		}
	}
}
