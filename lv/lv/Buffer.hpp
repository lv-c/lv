// *********************************************************************
//  Buffer   version:  1.0   ·  date: 11/18/2007
//  --------------------------------------------------------------------
//		用 vector<char> 作为 Buffer 类
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BUFFER_HPP
#define LV_BUFFER_HPP

#include <boost/assert.hpp>
#include <boost/range.hpp>

#include <vector>
#include <memory>
#include <iterator>
#include <type_traits>


namespace lv
{
	typedef std::vector<char>	Buffer;
	typedef std::shared_ptr<Buffer>	BufferPtr;
	typedef std::shared_ptr<Buffer const>	ConstBufferPtr;


	/// these two classes are compatible with boost.range and boost.BOOST_FOREACH
	template <typename T> class ConstBufferRefT;
	template <typename T> class BufferRefT;

	typedef ConstBufferRefT<char>	ConstBufferRef;
	typedef BufferRefT<char>	BufferRef;


	template <typename T>
	class BufferRefT
	{
		T * data_;
		size_t size_;

		template<typename> friend class ConstBufferRefT;

		std::shared_ptr<std::vector<T> >	holder_;

	public:

		typedef T & reference;
		typedef T const & const_reference;

		typedef T * iterator;
		typedef T const * const_iterator;

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
			: holder_(buf_ptr)
			, data_(buf_ptr->empty() ? nullptr : &(*buf_ptr)[0])
			, size_(buf_ptr->size())
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
	};


	template <typename T>
	class ConstBufferRefT
	{
		T const * data_;
		size_t	size_;

		std::shared_ptr<std::vector<T> >	holder_;

	public:
		typedef T const & const_reference;


		typedef T const * iterator;
		typedef T const * const_iterator;


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
			: holder_(buf_ptr)
			, data_(buf_ptr->empty() ? nullptr : &(*buf_ptr)[0])
			, size_(buf_ptr->size())
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

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value>::type	append(Buffer & buf, T t)
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

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value>::type	insert(Buffer & buf, size_t pos, T t)
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

			std::copy(static_cast<char const *>(data), static_cast<char const *>(data) + size, 
				stdext::make_checked_array_iterator(buf.data(), buf.size(), pos));
		}

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value>::type	write(BufferRef buf, size_t pos, T t)
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

			std::copy(buf.data() + pos, buf.data() + pos + size, 
				stdext::make_checked_array_iterator(static_cast<char *>(data), size));
		}

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value>::type	read(ConstBufferRef const & buf, size_t pos, T & t)
		{
			read(buf, pos, &t, sizeof(t));
		}

		template<typename T>
		T	read(ConstBufferRef const & buf, size_t pos)
		{
			T t;
			read(buf, pos, t);

			return t;
		}

		//

		inline char * data(Buffer & buf)
		{
			return buf.empty() ? nullptr : &buf[0];
		}

		inline char const * data(Buffer const & buf)
		{
			return buf.empty() ? nullptr : &buf[0];
		}

		inline char * data(BufferPtr const & buf)
		{
			return buf->empty() ? nullptr : &(*buf)[0];
		}

		inline std::string	to_string(ConstBufferRef const & buf)
		{
			return std::string(buf.data(), buf.size());
		}
	}
}

#endif
