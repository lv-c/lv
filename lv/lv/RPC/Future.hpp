// *********************************************************************
//  Future   version:  1.0   ¡¤  date: 09/25/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_FUTURE_HPP
#define LV_RPC_FUTURE_HPP

#include <lv/Concurrent/Future.hpp>


namespace lv { namespace rpc {
	
	namespace detail
	{
		template<class ArchivePair>
		class IPromise
		{
		public:

			virtual	void	set(typename ArchivePair::iarchive_type & ia) = 0;

			virtual	void	set_exception(std::exception_ptr ex) = 0;
		};

		template<typename Ret, class ArchivePair>
		struct ValueWrapper
		{
			Ret		value;

			void	load(typename ArchivePair::iarchive_type & ia, Promise<Ret> & promise)
			{
				ia >> value;
				promise.set_value(&value);
			}
		};

		template<class ArchivePair>
		struct ValueWrapper<void, ArchivePair>
		{
			void	load(typename ArchivePair::iarchive_type & ia, Promise<void> & promise)
			{
				promise.set_value(nullptr);
			}
		};

		template<typename Ret, class ArchivePair>
		class ReturnPromise : public IPromise<ArchivePair>
		{
			std::shared_ptr<Promise<Ret> > promise_;

		public:

			ReturnPromise()
				: promise_(std::make_shared<Promise<Ret> >())
			{
			}

			Future<Ret>	get_future()
			{
				return promise_->get_future();
			}

			/**
			 * @exception boost::archive::archive_exception ?
			 */
			virtual void	set(typename ArchivePair::iarchive_type & ia)
			{
				ValueWrapper<Ret, ArchivePair> val;
				val.load(ia, *promise_);
			}

			virtual	void	set_exception(std::exception_ptr ex)
			{
				promise_->set_exception(ex);
			}
		};
	}
} }

#endif