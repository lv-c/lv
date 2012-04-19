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

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>

namespace lv
{
	typedef std::vector<char>	Buffer;
	typedef boost::shared_ptr<Buffer>	BufferPtr;
	typedef boost::shared_ptr<Buffer const>	ConstBufferPtr;


	/// note : these two classes are compatible with boost.range and boost.foreach
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

		boost::shared_ptr<std::vector<T> >	holder_;

	public:

		typedef T & reference;
		typedef T const & const_reference;

		typedef T * iterator;
		typedef T const * const_iterator;

		BufferRefT()
			: data_(NULL)
			, size_(0)
		{
		}

		BufferRefT(T * data, size_t size)
			: data_(data)
			, size_(size)
		{
		}

		BufferRefT(std::vector<T> & buf)
			: data_(buf.empty() ? NULL : &buf[0])
			, size_(buf.size())
		{
		}

		BufferRefT(boost::shared_ptr<std::vector<T> > & buf_ptr)
			: holder_(buf_ptr)
			, data_(buf_ptr->empty() ? NULL : &(*buf_ptr)[0])
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
			return data_ == NULL || size_ == 0;
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
			BOOST_ASSERT(data_ != NULL);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}

		const_reference operator[] (size_t index) const
		{
			BOOST_ASSERT(data_ != NULL);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}
	};


	template <typename T>
	class ConstBufferRefT
	{
		T const * data_;
		size_t	size_;

		boost::shared_ptr<std::vector<T> >	holder_;

	public:
		typedef T const & const_reference;


		typedef T const * iterator;
		typedef T const * const_iterator;


		ConstBufferRefT()
			: data_(NULL)
			, size_(0)
		{
		}

		ConstBufferRefT(T const * data, size_t size)
			: data_(data)
			, size_(size)
		{
		}

		ConstBufferRefT(std::vector<T> const & buf)
			: data_(buf.empty() ? NULL : &buf[0])
			, size_(buf.size())
		{
		}

		ConstBufferRefT(boost::shared_ptr<std::vector<T> > const & buf_ptr)
			: holder_(buf_ptr)
			, data_(buf_ptr->empty() ? NULL : &(*buf_ptr)[0])
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
			return data_ == NULL || size_ == 0;
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
			BOOST_ASSERT(data_ != NULL);
			BOOST_ASSERT(index < size_);
			return data_[index];
		}
	};


	namespace buffer
	{
		// Write to the end of the buffer . You'd better reserve enough space to prevent frequent
		// memory allocation.
		inline void append(Buffer & buf, void const* data, size_t size)
		{
			buf.insert(buf.end(), static_cast<char const*>(data), static_cast<char const*>(data) + size);
		}

		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type	append(Buffer & buf, T t)
		{
			append(buf, &t, sizeof(t));
		}


		/// @exception std::out_of_range
		inline	void write(BufferRef buf, size_t pos, void const* data, size_t size)
		{
			if(pos + size > buf.size())
			{
				throw std::out_of_range("buffer::write out of range");
			}

			std::copy(static_cast<char const *>(data), static_cast<char const *>(data) + size, buf.data() + pos);
		}

		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type	write(BufferRef buf, size_t pos, T t)
		{
			write(buf, pos, &t, sizeof(t));
		}
		
		/// @exception std::out_of_range
		inline	void read(ConstBufferRef const & buf, size_t pos, void * data, size_t size)
		{
			if(pos + size > buf.size())
			{
				throw std::out_of_range("buffer::read out of range");
			}

			std::copy(buf.data() + pos, buf.data() + pos + size, static_cast<char *>(data));
		}

		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type	read(ConstBufferRef const & buf, size_t pos, T & t)
		{
			read(buf, pos, &t, sizeof(t));
		}

		inline char * data(Buffer & buf)
		{
			return &buf[0];
		}

		inline char const * data(Buffer const & buf)
		{
			return &buf[0];
		}

		inline char * data(BufferPtr const & buf)
		{
			return &(*buf)[0];
		}

		inline std::string	to_string(ConstBufferRef const & buf)
		{
			return std::string(buf.data(), buf.size());
		}
	}
}


#endif // LV_BUFFER_HPP
