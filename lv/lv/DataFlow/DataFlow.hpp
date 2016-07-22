// *********************************************************************
//  DataFlow   version:  1.0   ��  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_DATAFLOW_HPP
#define LV_DATAFLOW_DATAFLOW_HPP

#include <lv/Buffer.hpp>

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Connection.hpp>

#include <boost/noncopyable.hpp>

#include <map>
#include <functional>
#include <shared_mutex>


namespace lv { namespace flow {

	// thread-safe and the data flow is running concurrently when 
	// push_impl is called in multiple threads.
	template<template<class> class PushPolicy, class Port, class Compare>
	class DataFlow : boost::noncopyable
	{
		typedef std::pair<Port, BufferPtr>	port_buffer_pair;

	public:

		typedef PushPolicy<port_buffer_pair>	push_policy_type;

	private:

		struct ConnectionSlotPair
		{
			std::shared_ptr<detail::ConnectionImpl>	conn;
			slot_type	slot;
		};

		typedef std::multimap<Port, ConnectionSlotPair, Compare>  slots_map_type;
		slots_map_type	slots_;

		
		push_policy_type	push_policy_;

		std::shared_timed_mutex shared_mutex_;

	public:

		DataFlow(push_policy_type const & policy = push_policy_type())
			: push_policy_(policy)
		{
			push_policy_.set_callback(std::bind(&DataFlow::push_impl, this, std::placeholders::_1));
		}


		Connection	connect(Port const & port, slot_type const & slot)
		{
			ConnectionSlotPair connection_slot;
			connection_slot.slot = slot;

			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(shared_mutex_);

			slots_map_type::iterator it = slots_.insert(std::make_pair(port, connection_slot));
			it->second.conn = std::make_shared<detail::ConnectionImpl>(this, &DataFlow::disconnect_fun, it);

			return Connection(it->second.conn);
		}

		void	push(Port const & port, BufferPtr buf)
		{
			push_policy_(std::make_pair(port, buf));
		}

		// clear all the slots
		void	clear()
		{
			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(shared_mutex_);

			slots_.clear();
		}

	private:


		void	push_impl(port_buffer_pair const & port_buf)
		{
			// a read lock
			std::shared_lock<std::shared_timed_mutex> lock(shared_mutex_);

			auto ret = slots_.equal_range(port_buf.first);

			for (slots_map_type::iterator it = ret .first; it != ret .second; ++it)
			{
				it->second.slot(port_buf.second);
			}
		}


		static void disconnect_fun(void * obj, boost::any const & iter)
		{
			DataFlow * dataflow = reinterpret_cast<DataFlow*>(obj);
			
			slots_map_type::iterator it = boost::any_cast<slots_map_type::iterator>(iter);
			
			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(dataflow->shared_mutex_);

			dataflow->slots_.erase(it);
		}

	};

} }


#endif