// *********************************************************************
//  StringGather   version:  1.0   ¡¤  date: 11/30/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_LOG_STRINGGATHER_HPP
#define LV_LOG_STRINGGATHER_HPP

#include <sstream>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Gather.hpp>
#include <lv/SharedPtr.hpp>

namespace lv { namespace log {
	
	class StringGather : public Gather
	{
	public:
		
		typedef std::function<void(log::string_type &&, int)> receiver_t;

	protected:

		std::basic_ostringstream<log::char_type> 	oss_;

		receiver_t	receiver_;

	public:

		StringGather(receiver_t receiver, filter_type filter = filter_type())
			: Gather(lv::shared_from_object(oss_), filter)
			, receiver_(receiver)
		{
		}

	protected:

		void	on_record_end(int lvl) override
		{
			LV_SCOPE_EXIT([this] { mutex_.unlock(); });

			end_record(lvl);

			on_receive(oss_.str(), lvl);

			// empty the stringstream
			oss_.str(log::string_type());
		}

		virtual	void	on_receive(string_type && str, int lvl)
		{
			if (receiver_)
			{
				receiver_(std::move(str), lvl);
			}
		}
	};
} }

#endif