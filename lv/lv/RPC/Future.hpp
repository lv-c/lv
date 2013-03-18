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

#include <boost/thread/future.hpp>

namespace lv { namespace rpc {
	
	// Not intended to be used by the user.
	namespace detail
	{
		template<class ArchivePair>
		class PromiseBase
		{
		public:

			virtual	void	set(typename ArchivePair::iarchive_type & ia) = 0;

			virtual	void	set_exception(boost::exception_ptr ex) = 0;
		};

		template<class ArchivePair>
		class AchnowPromise : public PromiseBase<ArchivePair>
		{
			boost::promise<void> promise_;

		public:

			boost::shared_future<void>	get_future()
			{
				return boost::move(promise_.get_future());
			}

			virtual void	set(typename ArchivePair::iarchive_type & ia) 
			{
				promise_.set();
			}

			virtual	void	set_exception(boost::exception_ptr ex)
			{
				promise_.set_exception(ex);
			}
		};

		template<typename Ret, class ArchivePair>
		class ReturnPromise : public PromiseBase<ArchivePair>
		{
			boost::promise<Ret>	promise_;

		public:

			boost::shared_future<Ret>	get_future()
			{
				return boost::move(promise_.get_future());
			}

			/**
			 * @exception boost::archive::archive_exception ?
			 */
			virtual void	set(typename ArchivePair::iarchive_type & ia)
			{
				Ret ret;
				ia >> ret;
				promise_.set_value(ret);
			}

			virtual	void	set_exception(boost::exception_ptr ex)
			{
				promise_.set_exception(ex);
			}
		};
	}
	

	class Acknowledgment : public boost::shared_future<void>
	{
		template<typename, class> friend class Client;
		
		Acknowledgment(boost::shared_future<void> const & future)
			: boost::shared_future<void>(future)
		{
		}

	public:

		/// creates an empty acknowledgment
		Acknowledgment() {}
	};


	template<typename Ret>
	class ReturningHandler : public boost::shared_future<Ret>
	{
		template<typename, class> friend class Client;

		ReturningHandler(boost::shared_future<Ret> const & future)
			: boost::shared_future<Ret>(future)
		{
		}

	public:

		typedef Ret	value_type;

		/// creates an empty returning handler
		ReturningHandler() {}
	};

} }

#endif