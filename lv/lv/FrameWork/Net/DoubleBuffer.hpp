// *********************************************************************
//  DoubleBuffer   version:  1.0   ¡¤  date: 2018/08/10
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2018 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Buffer.hpp>

#include <boost/assert.hpp>

#include <array>
#include <mutex>


namespace lv::net
{

	// thread-safe

	class DoubleBuffer
	{
	public:

		class ScopedLock
		{
			DoubleBuffer *	obj_;

			Buffer const *	buf_;

		public:

			ScopedLock(DoubleBuffer * obj, Buffer const * buf);

			ScopedLock(ScopedLock && other);

			~ScopedLock();


			operator bool() const;

			Buffer const *	buffer() const;
		};

	private:

		std::array<Buffer, 2>	buffers_;


		static constexpr size_t		NullLock = -1;


		size_t		write_index_ = 0;

		size_t		lock_index_ = NullLock;


		std::mutex	mutex_;

	public:

		void	reset();


		void	put(ConstBufferRef buf);


		Buffer const *	lock();

		void	unlock(Buffer const & buf);


		ScopedLock	scope_lock();

	};
}