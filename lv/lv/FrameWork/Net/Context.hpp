// *********************************************************************
//  Context   version:  1.0   ¡¤  date: 01/03/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/IOContextWrapper.hpp>


namespace lv::net
{
	class Context
	{
	protected:

		IOContextWrapper		io_wrapper_;

	public:

		explicit Context(IOContextWrapper const & io_wrapper)
			: io_wrapper_(io_wrapper)
		{
		}

		virtual	~Context() = default;

		asio::io_context &	io_context() const
		{
			return io_wrapper_.io_context();
		}

		bool	has_strand() const
		{
			return io_wrapper_.has_strand();
		}

		asio::io_context::strand &	strand() const
		{
			return io_wrapper_.strand();
		}

		IOContextWrapper const &	io_wrapper() const
		{
			return io_wrapper_;
		}
	};

}
