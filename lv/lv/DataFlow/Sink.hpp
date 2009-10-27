// *********************************************************************
//  Sink   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_SINK_HPP
#define LV_DATAFLOW_SINK_HPP

#include <lv/Buffer.hpp>

#include <lv/DataFlow/Registery.hpp>

#include <memory>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace lv { namespace flow {

	typedef boost::function<void(slot_type, BufferPtr)> proxy_push_type;

	/// thread-safe
	template<template<class> class PushPolicy, class Key, class IArchive>
	class Sink : boost::noncopyable
	{
		typedef Registery<Key, IArchive> registery_type;
		std::auto_ptr<registery_type> registery_;


		typedef PushPolicy<BufferPtr> push_policy_type;
		push_policy_type	push_policy_;

	public:

		/**
		 * @proxy_push you can handle exceptions using a proxy push
		 * @param reg use std::auto_ptr to indicate that the ownership of @a reg will be 
		 *	transfered and no functions should be registered any more.
		 */
		Sink(std::auto_ptr<registery_type> reg, proxy_push_type const & proxy_push = proxy_push_type(), 
				push_policy_type const & policy = push_policy_type())
			: registery_(reg) 
			, push_policy_(policy)
		{
			registery_->end_reg();

			slot_type slot = boost::bind(&Sink::push_impl, this, _1);

			if(proxy_push)
			{
				push_policy_.set_callback(boost::bind(proxy_push, slot, _1));
			}
			else
			{
				push_policy_.set_callback(slot);
			}
		}

		void	push(BufferPtr buf)
		{
			push_policy_(buf);
		}

	private:

		void	push_impl(BufferPtr buf)
		{
			boost::iostreams::filtering_istream raw_is(boost::make_iterator_range(*buf));
			IArchive ia(raw_is);

			registery_->invoke(ia);
		}
	};

} }

#endif