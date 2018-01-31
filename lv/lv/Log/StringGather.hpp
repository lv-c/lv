// *********************************************************************
//  StringGather   version:  1.0   ¡¤  date: 11/30/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <sstream>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Gather.hpp>
#include <lv/SharedPtr.hpp>

namespace lv::log
{
	class StringGather : public Gather
	{
	public:
		
		using receiver_t = std::function<void(log::string_type &&, int)>;

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

			string_type str = oss_.str();
			oss_.str(string_type());

			on_receive(std::move(str), lvl);
		}

		virtual	void	on_receive(string_type && str, int lvl)
		{
			if (receiver_)
			{
				receiver_(std::move(str), lvl);
			}
		}
	};
}