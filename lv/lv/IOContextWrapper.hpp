// *********************************************************************
//  IOContextWrapper   version:  1.0   ¡¤  date: 2012/03/28
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/assert.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>

#include <memory>


namespace lv
{
	class IOContextWrapper
	{
		boost::asio::io_context *	io_;

		boost::asio::io_context::strand *	strand_;

	public:

		IOContextWrapper()
			: io_(nullptr)
			, strand_(nullptr)
		{
		}

		IOContextWrapper(boost::asio::io_context & io)
			: io_(&io)
			, strand_(nullptr)
		{
		}

		IOContextWrapper(boost::asio::io_context::strand & strand)
			: io_(nullptr)
			, strand_(&strand)
		{
		}

		operator bool() const
		{
			return (io_ != nullptr || strand_ != nullptr);
		}

		boost::asio::io_context &	io_context() const
		{
			if (io_ != nullptr)
			{
				return *io_;
			}
			else
			{
				return strand_->get_io_context();
			}
		}

		bool	has_strand() const
		{
			return (strand_ != nullptr);
		}

		boost::asio::io_context::strand &	strand() const
		{
			BOOST_ASSERT(strand_ != nullptr);
			return *strand_;
		}

		template<class Handler>
		void	post(Handler && handler) const
		{
			BOOST_ASSERT(*this);

			if (io_ != nullptr)
			{
				io_->post(std::forward<Handler>(handler));
			}
			else
			{
				strand_->post(std::forward<Handler>(handler));
			}
		}
	};
}