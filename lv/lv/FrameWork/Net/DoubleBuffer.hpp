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


		size_t		write_index_;

		size_t		lock_index_;


		bool		shutdown_;


		std::mutex	mutex_;

	public:

		DoubleBuffer();

		void	reset();


		void	put(ConstBufferRef buf);

		void	shutdown();


		Buffer const *	lock(bool * need_shutdown = nullptr);

		void	unlock(Buffer const & buf);


		ScopedLock	scope_lock();

	};
}