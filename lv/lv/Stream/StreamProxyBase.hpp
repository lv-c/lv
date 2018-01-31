// *********************************************************************
//  StreamProxyBase   version:  1.0   ¡¤  date: 04/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Stream/StreamBase.hpp>
#include <lv/Stream/CallForwarding.hpp>

#include <ios>

#include <boost/assert.hpp>


namespace lv
{

	class StreamProxyBase
	{
		std::ios *	std_ios_;

		StreamBase * 	lv_ios_;

	protected:

		StreamProxyBase()
			: std_ios_(nullptr)
			, lv_ios_(nullptr)
		{
		}

		StreamProxyBase(std::ios & ios)
			: std_ios_(&ios)
			, lv_ios_(nullptr)
		{
		}

		StreamProxyBase(StreamBase & ios)
			: std_ios_(nullptr)
			, lv_ios_(&ios)
		{
		}

		void	set(std::ios * std_ios, StreamBase * lv_ios)
		{
			BOOST_ASSERT(std_ios == nullptr || lv_ios == nullptr);

			this->std_ios_ = std_ios;
			this->lv_ios_ = lv_ios;
		}

	public:

		using iostate = std::ios_base::iostate;
		using failure = std::ios_base::failure;

		using streamsize = std::streamsize;
		using streamoff = std::streamoff;
		using streampos = std::streampos;


		iostate	exceptions() const
		{
			LV_STREAMPROXY_FW_CALL_0(exceptions, return);
		}

		void	exceptions(iostate except)
		{
			LV_STREAMPROXY_FW_CALL_1(exceptions, except, (void));
		}

		iostate	rdstate() const
		{
			LV_STREAMPROXY_FW_CALL_0(rdstate, return);
		}

		void	setstate(iostate state)
		{
			LV_STREAMPROXY_FW_CALL_1(setstate, state, (void));
		}

		void	clear(iostate state = std::ios_base::goodbit)
		{
			LV_STREAMPROXY_FW_CALL_1(clear, state, (void));
		}

		bool	good() const
		{
			LV_STREAMPROXY_FW_CALL_0(good, return);
		}

		bool	eof() const
		{
			LV_STREAMPROXY_FW_CALL_0(eof, return);
		}

		bool	fail() const
		{
			LV_STREAMPROXY_FW_CALL_0(fail, return);
		}

		bool	bad() const
		{
			LV_STREAMPROXY_FW_CALL_0(bad, return);
		}

		bool	operator ! () const
		{
			LV_STREAMPROXY_FW_CALL_0(operator !, return);
		}

	};

}
