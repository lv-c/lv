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
#include <lv/RPC/Exception.hpp>

namespace lv { namespace rpc {

	
	DEFINE_EXCEPTION_MSG(InvalidRPCHeader, std::runtime_error);


	// Not intended to be used by the user.
	namespace detail
	{
		template<class ArchivePair>
		class PromiseBase
		{
		public:

			/**
			 * @exception boost::archive::archive_exception ?
			 * @exception InvalidRPCHeader
			 * @exception ExceptionNotFoundError
			 */
			virtual	void set(typename ArchivePair::iarchive_t & ia) = 0;
		};

		template<class ArchivePair, class Pro>
		class AchnowPromise : public PromiseBase<ArchivePair>
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
			 * @copydoc PromiseBase::set
			 */
			virtual	void set(typename ArchivePair::iarchive_t & ia) 
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
					throw InvalidRPCHeader();
			}

	
		};

		template<typename Ret, class ArchivePair, class Pro>
		class ReturnPromise : PromiseBase<ArchivePair>
		{
			boost::promise<Ret>	promise_;

			Exceptions<ArchivePair, Pro> & exceptions_;

		private:
			template<typename> friend class ReturningHandler;

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
			 * @copydoc PromiseBase::set
			 */
			virtual	void set(typename ArchivePair::iarchive_t & ia)
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
					throw InvalidRPCHeader();
			}
		};
	}
	

	class Acknowledgment : public boost::future<void>
	{
		template<typename, class, class> friend class Client;

		template<class ArchivePair, class Pro>
		Acknowledgment(AchnowPromise<ArchivePair, Pro> const & promise)
			: boost::future<void>(promise.get())
		{
		}

	};


	template<typename Ret>
	class ReturningHandler : public boost::future<Ret>
	{
		template<typename, class, class> friend class Client;

		template<class ArchivePair, class Pro>
		ReturningHandler(ReturnPromise<Ret, ArchivePair, Pro> const & promise)
			: boost::future<Ret>(promise.get())
		{
		}
	};



} }


#endif // LV_RPC_FUTURE_HPP