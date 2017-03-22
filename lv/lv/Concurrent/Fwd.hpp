// *********************************************************************
//  Fwd   version:  1.0   ¡¤  date: 10/27/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_CONCURRENT_FWD_HPP
#define LV_CONCURRENT_FWD_HPP

namespace lv
{
	template<typename T>
	class FIFOQueue;

	template <typename T>
	class PriorityQueue;

	template <typename Task, template <typename> class QueuePolicy = FIFOQueue>
	class TaskQueue;

	class ThreadGroup;
}

#endif