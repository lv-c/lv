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

#include <lv/RPC/Future.hpp>
#include <lv/LazyInit.hpp>

#include <boost/thread/future.hpp>
#include <boost/signals2/signal.hpp>


namespace lv { namespace rpc {
	
	// Not intended to be used by the user.
	namespace detail
	{
		template<class ArchivePair>
		class IPromise
		{
		public:

			virtual	void	set(typename ArchivePair::iarchive_type & ia) = 0;

			virtual	void	set_exception(boost::exception_ptr ex) = 0;
		};

		template<typename Ret>
		class PromiseSignal
		{
			typedef boost::signals2::signal<void(boost::shared_future<Ret>)>	signal_type;

			LazeInit<signal_type>	signal_;

		public:

			boost::signals2::connection	connect(typename signal_type::slot_type const & slot)
			{
				return signal_.get().connect(slot);
			}

		protected:

			void	signal(boost::shared_future<Ret> future)
			{
				if(signal_)
				{
					signal_.get() (future);
				}
			}
		};

		template<typename Ret, class ArchivePair>
		struct ValueWrapper
		{
			Ret		value;

			void	load(typename ArchivePair::iarchive_type & ia, boost::promise<Ret> & promise)
			{
				ia >> value;
				promise.set_value(value);
			}

			Ret *	pointer()
			{
				return &value;
			}
		};

		template<class ArchivePair>
		struct ValueWrapper<void, ArchivePair>
		{
			void	load(typename ArchivePair::iarchive_type & ia, boost::promise<void> & promise)
			{
				promise.set_value();
			}

			void *	pointer()
			{
				return NULL;
			}
		};

		template<typename Ret, class ArchivePair>
		class ReturnPromise : public IPromise<ArchivePair>, public PromiseSignal<Ret>
		{
			boost::promise<Ret> promise_;

			boost::shared_future<Ret>	future_;

		public:

			ReturnPromise()
			{
				// promise_.get_future can only be called once
				future_ = promise_.get_future();
			}

			typedef Ret	value_type;

			boost::shared_future<Ret>	get_future()
			{
				return future_;
			}

			/**
			 * @exception boost::archive::archive_exception ?
			 */
			virtual void	set(typename ArchivePair::iarchive_type & ia)
			{
				ValueWrapper<Ret, ArchivePair> val;
				val.load(ia, promise_);

				signal(future_);
			}

			virtual	void	set_exception(boost::exception_ptr ex)
			{
				promise_.set_exception(ex);
				signal(future_);
			}
		};
	}


	template<typename Ret>
	class ReturningHandler : public boost::shared_future<Ret>
	{
		boost::weak_ptr<detail::PromiseSignal<Ret> >	signal_;

		template<typename, class> friend class Client;

		ReturningHandler(boost::shared_future<Ret> future, boost::shared_ptr<detail::PromiseSignal<Ret> > signal)
			: boost::shared_future<Ret>(future)
			, signal_(signal)
		{
		}

	public:

		typedef Ret	value_type;

		/// creates an empty returning handler
		ReturningHandler() {}

		// this callback function will be invoked within the same thread of Client::on_receive
		template<typename Slot>
		boost::signals2::connection	connect(Slot const & slot)
		{
			BOOST_AUTO(s, signal_.lock());

			if(s)
			{
				return s->connect(slot);
			}
			else
			{
				throw boost::broken_promise();
			}
		}
	};


	typedef ReturningHandler<void>	Acknowledgment;

} }

#endif