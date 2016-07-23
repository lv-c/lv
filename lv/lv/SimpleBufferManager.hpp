// *********************************************************************
//  SimpleBufferManager   version:  1.0   ¡¤  date: 12/03/2008
//  --------------------------------------------------------------------
//
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SIMPLEBUFFERMANAGER_HPP
#define LV_SIMPLEBUFFERMANAGER_HPP

#include <lv/IBufferManager.hpp>


namespace lv 
{

	class SimpleBufferManager : public IBufferManager
	{
		size_t	init_capacity_;

	public:

		explicit SimpleBufferManager(size_t init_capacity)
			: init_capacity_(init_capacity)
		{
		}

		/// return an empty buffer whose capacity is @a init_capacity_
		virtual BufferPtr	get()
		{
			BufferPtr buf = std::make_shared<Buffer>();
			buf->reserve(init_capacity_);

			return buf;
		}

	};

}

#endif