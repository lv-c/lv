// *********************************************************************
//  StreamFactory   version:  1.0   ��  date: 04/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Factory.hpp>


namespace lv::detail
{
	template<class T>
	class StreamFactory : public Factory<T>
	{
	protected:

		void	release(T * obj) override
		{
			obj->clear();
			obj->close();

			// patch. some bugs in boost.iostreams?
			if (obj->is_open())
			{
				delete obj;
				return;
			}

			Factory<T>::release(obj);
		}
	};
}
