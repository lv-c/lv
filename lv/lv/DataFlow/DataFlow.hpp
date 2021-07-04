// *********************************************************************
//  DataFlow   version:  1.0   ¡¤  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Connection.hpp>
#include <lv/Buffer.hpp>

#include <boost/noncopyable.hpp>

#include <map>
#include <shared_mutex>


namespace lv::flow
{
	// thread-safe and the data flow is running concurrently when 
	// push is called in multiple threads.
	template<class Port, class Compare>
	class DataFlow : boost::noncopyable
	{
		struct ConnectionSlotPair
		{
			std::shared_ptr<detail::ConnectionImpl>	conn;
			slot_type	slot;
		};

		using slots_map_type = std::multimap<Port, ConnectionSlotPair, Compare> ;
		slots_map_type	slots_;

		
		std::shared_timed_mutex shared_mutex_;

	public:

		Connection	connect(Port const & port, slot_type slot)
		{
			ConnectionSlotPair connection_slot;
			connection_slot.slot = std::move(slot);

			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(shared_mutex_);

			auto it = slots_.emplace(port, connection_slot);
			it->second.conn = std::make_shared<detail::ConnectionImpl>(this, &DataFlow::disconnect_fun, it);

			return Connection(it->second.conn);
		}

		void	push(Port port, BufferPtr buf)
		{
			// a read lock
			std::shared_lock<std::shared_timed_mutex> lock(shared_mutex_);

			auto ret = slots_.equal_range(port);

			for (auto it = ret.first; it != ret.second; ++it)
			{
				it->second.slot(buf);
			}
		}

		// clear all the slots
		void	clear()
		{
			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(shared_mutex_);

			slots_.clear();
		}

	private:

		static void disconnect_fun(void * obj, std::any const & iter)
		{
			DataFlow * dataflow = reinterpret_cast<DataFlow*>(obj);
			
			auto it = std::any_cast<typename slots_map_type::iterator>(iter);
			
			// a write lock
			std::lock_guard<std::shared_timed_mutex> lock(dataflow->shared_mutex_);

			dataflow->slots_.erase(it);
		}

	};

}

