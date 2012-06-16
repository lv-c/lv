// *********************************************************************
//  StringGather   version:  1.0   ¡¤  date: 11/30/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STRINGGATHER_HPP
#define LV_STRINGGATHER_HPP

#include <sstream>

#include <lv/Log/Fwd.hpp>
#include <lv/Log/Gather.hpp>
#include <lv/SharedPtr.hpp>

namespace lv { namespace log {
	
	class StringGather : public Gather
	{
	public:
		
		typedef boost::function<void(log::string_type const &, int)> receiver_t;

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

		virtual	void on_record_end(int lvl)
		{
			end_record(lvl);

			on_receive(oss_.str(), lvl);

			// empty the stringstream
			oss_.str(log::string_type());

			// unlock
			mutex_.unlock();
		}

		virtual	void on_receive(string_type const & str, int lvl)
		{
			if(receiver_)
			{
				receiver_(str, lvl);
			}
		}
	};
} }

#endif // LV_STRINGGATHER_HPP