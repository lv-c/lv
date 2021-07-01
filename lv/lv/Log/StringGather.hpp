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
		
		using receiver_t = std::function<void(string_type const &, int)>;

	protected:

		std::basic_ostringstream<char_type> 	oss_;

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

			string_type str = std::move(oss_).str();
			LV_SCOPE_EXIT([this, &str] {
				str.clear();
				oss_.str(std::move(str));
			});

			on_receive(str, lvl);
		}

		virtual	void	on_receive(string_type const & str, int lvl)
		{
			if (receiver_)
			{
				receiver_(str, lvl);
			}
		}
	};
}