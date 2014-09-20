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

#ifndef LV_STREAM_STREAMBASE_HPP
#define LV_STREAM_STREAMBASE_HPP

#include <ios>

#include <boost/noncopyable.hpp>

namespace lv
{
	class StreamBase : boost::noncopyable
	{
	public:
		typedef std::ios_base::iostate	iostate;
		typedef std::ios_base::failure	failure;

		typedef std::streamsize	streamsize;
		typedef std::streamoff	streamoff;
		typedef std::streampos	streampos;

		StreamBase()
			: state_(std::ios_base::goodbit)
			, except_(std::ios_base::goodbit)
		{
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
			if(state != std::ios_base::goodbit)
			{
				clear(rdstate() | state);
			}
		}

		void	clear(iostate state = std::ios_base::goodbit)
		{
			this->state_ = state;

			if((state_ & except_) == 0)
			{
			}
			else if(state_ & except_ & std::ios_base::badbit)
			{
				throw(failure("ios_base::badbit set"));
			}
			else if(state_ & except_ & std::ios_base::failbit)
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
			return (rdstate() & std::ios_base::eofbit);
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


	private:

		iostate	state_;		// stream state

		iostate	except_;	// exception mask
	};
}


#endif