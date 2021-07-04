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

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/Buffer.hpp>

#include <boost/noncopyable.hpp>

#include <array>
#include <mutex>


namespace lv::net
{

	// thread-safe

	class DoubleBuffer
	{
	public:

		class ScopedRead : boost::noncopyable
		{
			DoubleBuffer *	obj_;

			Buffer const *	buf_;

		public:

			ScopedRead(DoubleBuffer * obj, Buffer const * buf)
				: obj_(obj)
				, buf_(buf)
			{
			}

			~ScopedRead()
			{
				if (buf_ != nullptr)
				{
					obj_->end_read(*buf_);
				}
			}

			operator bool() const { return buf_ != nullptr; }

			Buffer const *	buffer() const { return buf_; }
		};

		class BatchWriter : boost::noncopyable
		{
			Buffer &	buf_;

		public:

			explicit BatchWriter(Buffer & buf) : buf_(buf) {}

			void	write(ConstBufferRef data) const
			{
				buffer::append(buf_, data);
			}
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


		void	write(ConstBufferRef data);

		// batch_write ensure that all the data will be written into the same buffer
		template<class Fn>
		void	batch_write(Fn && fn)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			if (!shutdown_)
			{
				fn(BatchWriter(buffers_[write_index_]));
			}
		}


		Buffer const *	start_read(bool * need_shutdown = nullptr);

		void	end_read(Buffer const & buf);

		ScopedRead	scoped_read();


		void	reset();

		void	shutdown();
	};
}
