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

#include <lv/Factory.hpp>
#include <lv/IBufferManager.hpp>

namespace lv 
{

	class SimpleBufferManager : public IBufferManager
	{

		size_t	init_capacity_;

		typedef Factory<Buffer>	factory_type;
		boost::shared_ptr<factory_type>	factory_;

	public:

		explicit SimpleBufferManager(size_t init_capacity, 
			boost::shared_ptr<factory_type> factory = boost::shared_ptr<factory_type>(new factory_type()))
			: init_capacity_(init_capacity)
			, factory_(factory)
		{
		}


		/// return an empty buffer whose capacity is at lease @a init_capacity_
		virtual BufferPtr	get()
		{
			BufferPtr buf = factory_->get();
			
			if(! buf->empty())
			{
				buf->clear();
				BOOST_ASSERT(buf->capacity() >= init_capacity_);
			}

			buf->reserve(init_capacity_);

			return buf;
		}

	};

}

#endif