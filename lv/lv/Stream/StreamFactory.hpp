// *********************************************************************
//  StreamFactory   version:  1.0   ��  date: 04/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAM_STREAMFACTORY_HPP
#define LV_STREAM_STREAMFACTORY_HPP

#include <lv/Factory.hpp>

namespace lv { namespace detail {
	
	template<typename T>
	class StreamFactory : public Factory<T>
	{
	protected:

		virtual void	release(T * obj)
		{
			obj->clear();
			obj->close();

			// patch. some bugs in boost.iostreams?
			if(obj->is_open())
			{
				delete obj;
				return;
			}

			Factory::release(obj);
		}
	};
} }

#endif