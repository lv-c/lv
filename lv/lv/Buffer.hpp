// *********************************************************************
//  Buffer   version:  1.0   ·  date: 11/18/2007
//  --------------------------------------------------------------------
//		用 vector<char> 作为 Buffer 类
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Ensure.hpp>
#include <lv/Concepts.hpp>

#include <vector>
#include <memory>
#include <span>


namespace lv
{
	using Buffer = std::vector<char>;
	using BufferPtr = std::shared_ptr<Buffer>;

	using BufferRef = std::span<char>;
	using ConstBufferRef = std::span<char const>;

	namespace buffer
	{
		// append

		// Write to the end of the buffer . You'd better reserve enough space to prevent frequent
		// memory allocation.
		inline void append(Buffer & buf, void const * data, size_t size)
		{
			buf.insert(buf.end(), static_cast<char const *>(data), static_cast<char const *>(data) + size);
		}

		inline void	append(Buffer & buf, ConstBufferRef data)
		{
			buf.insert(buf.end(), data.begin(), data.end());
		}

		template<TriviallyCopyable T>
		void	append(Buffer & buf, T const & t)
		{
			append(buf, &t, sizeof(t));
		}


		// insert

		inline void	insert(Buffer & buf, size_t pos, void const * data, size_t size)
		{
			LV_ENSURE(pos <= buf.size(), std::out_of_range("buffer::insert out of range"));

			buf.insert(buf.begin() + pos, static_cast<char const *>(data), static_cast<char const *>(data) + size);
		}

		template<TriviallyCopyable T>
		void	insert(Buffer & buf, size_t pos, T const & t)
		{
			insert(buf, pos, &t, sizeof(t));
		}


		// write

		/// @exception std::out_of_range
		inline void	write(BufferRef buf, size_t pos, void const * data, size_t size)
		{
			LV_ENSURE(pos + size <= buf.size(), std::out_of_range("buffer::write out of range"));

			std::copy(static_cast<char const *>(data), static_cast<char const *>(data) + size, buf.data() + pos);
		}

		template<TriviallyCopyable T>
		void	write(BufferRef buf, size_t pos, T const & t)
		{
			write(buf, pos, &t, sizeof(t));
		}


		// read
		
		/// @exception std::out_of_range
		inline void	read(ConstBufferRef buf, size_t pos, void * data, size_t size)
		{
			LV_ENSURE(pos + size <= buf.size(), std::out_of_range("buffer::read out of range"));

			std::copy(buf.data() + pos, buf.data() + pos + size, static_cast<char *>(data));
		}

		template<TriviallyCopyable T>
		void	read(ConstBufferRef buf, size_t pos, T & t)
		{
			read(buf, pos, &t, sizeof(t));
		}

		template<class T>
		T	read(ConstBufferRef buf, size_t pos)
		{
			T t;
			read(buf, pos, t);

			return t;
		}

		//
		constexpr std::string_view	view(ConstBufferRef buf)
		{
			return std::string_view(buf.data(), buf.size());
		}

		template<TriviallyCopyable T>
		constexpr BufferRef		from_object(T & t)
		{
			return BufferRef(reinterpret_cast<char *>(&t), sizeof(t));
		}

		template<TriviallyCopyable T>
		constexpr ConstBufferRef	from_object(T const & t)
		{
			return ConstBufferRef(reinterpret_cast<char const *>(&t), sizeof(t));
		}
	}
}
