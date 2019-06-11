// *********************************************************************
//  StreamBase   version:  1.0   ��  date: 04/02/2010
//  --------------------------------------------------------------------
//  �ṩ�ͱ�׼ C++ ��ͳһ�Ľӿڣ���һ���ṩ���нӿڣ�
//	��׼ C++ ������boost.iostreams�����ڳ�ʼ��ʱ��̬�����ڴ棨locale,
//		Mutex ֮�ࣩ�������Ƶ���ش����µ������� ������Ϊ��Ҫ�Ŀ�����
//	ʹ�� IBufferStream/OBufferStream ���Ա�����Щ����
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <ios>

#include <boost/noncopyable.hpp>

namespace lv
{
	class StreamBase : boost::noncopyable
	{
	public:

		using iostate = std::ios_base::iostate;
		using failure = std::ios_base::failure;

		using streamsize = std::streamsize;
		using streamoff = std::streamoff;
		using streampos = std::streampos;

		StreamBase()
			: state_(std::ios_base::goodbit)
			, except_(std::ios_base::goodbit)
		{
		}

		StreamBase(StreamBase && other) noexcept
		{
			*this = std::forward<StreamBase>(other);
		}

		StreamBase & operator = (StreamBase && other) noexcept
		{
			if (this != &other)
			{
				state_ = other.state_;
				except_ = other.except_;
			}

			return *this;
		}


		iostate	exceptions() const
		{
			return except_;
		}

		void	exceptions(iostate except)
		{
			this->except_ = except;
			clear(state_);
		}

		iostate	rdstate() const
		{
			return state_;
		}

		void	setstate(iostate state)
		{
			// merge in state argument
			if (state != std::ios_base::goodbit)
			{
				clear(rdstate() | state);
			}
		}

		void	clear(iostate state = std::ios_base::goodbit)
		{
			this->state_ = state;

			if ((state_ & except_) == 0)
			{
			}
			else if (state_ & except_ & std::ios_base::badbit)
			{
				throw(failure("ios_base::badbit set"));
			}
			else if (state_ & except_ & std::ios_base::failbit)
			{
				throw(failure("ios_base::failbit set"));
			}
			else
			{
				throw(failure("ios_base::eofbit set"));
			}
		}

		bool	good() const
		{
			return (rdstate() == std::ios_base::goodbit);
		}

		bool	eof() const
		{
			return ((rdstate() & std::ios_base::eofbit) != 0);
		}

		bool	fail() const
		{
			return ((rdstate() & (std::ios_base::badbit | std::ios_base::failbit)) != 0);
		}

		bool	bad() const
		{
			return ((rdstate() & std::ios_base::badbit) != 0);
		}

		bool	operator ! () const
		{
			return fail();
		}

		explicit operator bool() const
		{
			return !fail();
		}


	private:

		iostate	state_;		// stream state

		iostate	except_;	// exception mask
	};
}
