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

#include <lv/Log/Gather.hpp>
#include <lv/SharedPtr.hpp>

namespace lv
{
	class StringGather : public Gather
	{
	public:
		
		typedef boost::function<void(std::string const &, log::level)> receiver_t;

	protected:

		std::ostringstream	oss_;

		receiver_t	receiver_;

	public:

		StringGather(receiver_t receiver, filter_t filter = filter_t())
			: Gather(lv::shared_from_object(oss_), filter)
			, receiver_(receiver)
		{
		}

	protected:

		virtual	void on_record_end(log::level lvl)
		{
			Gather::on_record_end(lvl);

			if(receiver_)
				receiver_(oss_.str(), lvl);
			// empty the stringstream
			oss_.str(std::string());
		}
	};
}

#endif // LV_STRINGGATHER_HPP