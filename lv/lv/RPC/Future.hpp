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

#include <boost/future.hpp>

#include <lv/Exception.hpp>
#include <lv/rpc/Exceptions.hpp>
#include <lv/rpc/Common.hpp>

namespace lv { namespace rpc {

	
	// fwd
	template<typename> class ReturningHandler;
	class Acknowledgment;

	// Not intended to be used by the user.
	namespace detail
	{
		template<class ArchivePair, class Pro>
		class AchnowPromise
		{
			boost::promise<void> promise_;

			Exceptions<ArchivePair, Pro> &	exceptions_;
		
		private:
			friend class Acknowledgment;

			boost::promise<void> const & get() const
			{
				return promise_;
			}

		public:

			AchnowPromise(Exceptions<ArchivePair, Pro> & except)
				: exceptions_(except)
			{
			}

			/**
			 * @exception boost::archive::archive_exception ?
			 * @exception InvalidProtocolValue
			 * @exception InvalidExceptionID
			 */
			void operator () (typename ArchivePair::iarchive_t & ia) 
			{
				Pro::except::type ex;
				ia >> ex;
				if(ex == Pro::except::has_ex)
				{
					Pro::except_key_type key;
					ia >> key;
					promise_.set_exception(exceptions_.get(key, ia));
				}
				else if(ex == Pro::except::no_ex)
					promise_.set();
				else
					throw InvalidProtocolValue("invalid Pro::except value");
			}
		};

		template<typename Ret, class ArchivePair, class Pro>
		class ReturnPromise
		{
			boost::promise<Ret>	promise_;

			Exceptions<ArchivePair, Pro> & exceptions_;

		private:
			friend class ReturningHandler<Ret>;

			boost::promise<Ret> const & get() const
			{
				return promise_;
			}

		public:

			ReturnPromise(Exceptions<ArchivePair, Pro> & except)
				: exceptions_(except)
			{
			}

			/**
			 * @exception boost::archive::archive_exception ?
			 * @exception InvalidProtocolValue
			 * @exception InvalidExceptionID
			 */
			void operator () (typename ArchivePair::iarchive_t & ia)
			{
				Pro::except::type ex;
				ia >> ex;
				if(ex == Pro::except::has_ex)
				{
					Pro::except_key_type key;
					ia >> key;
					promise_.set_exception(exceptions_.get(key, ia));
				}
				else if(ex == Pro::except::no_ex)
				{
					Ret ret;
					ia >> ret;
					promise_.set(ret);
				}
				else
					throw InvalidProtocolValue("invalid Pro::except value");
			}
		};
	}
	

	class Acknowledgment : public boost::future<void>
	{
		template<typename, class, class> friend class Client;
		
		template<class ArchivePair, class Pro>
		Acknowledgment(detail::AchnowPromise<ArchivePair, Pro> const & promise)
			: boost::future<void>(promise.get())
		{
		}

	public:

		/// creates an empty acknowledgment
		Acknowledgment() {}
	};


	template<typename Ret>
	class ReturningHandler : public boost::future<Ret>
	{
		template<typename, class, class> friend class Client;

		template<class ArchivePair, class Pro>
		ReturningHandler(detail::ReturnPromise<Ret, ArchivePair, Pro> const & promise)
			: boost::future<Ret>(promise.get())
		{
		}

	public:

		/// creates an empty returning handler
		ReturningHandler() {}
	};



} }


#endif // LV_RPC_FUTURE_HPP