// *********************************************************************
//  Client   version:  1.0   ¡¤  date: 09/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING

#ifndef LV_RPC_CLIENT_HPP
#define LV_RPC_CLIENT_HPP

#include <lv/Config.hpp>
#include <lv/Exception.hpp>

#include <lv/RPC/Config.hpp>
#include <lv/RPC/RpcBase.hpp>
#include <lv/RPC/Future.hpp>
#include <lv/RPC/Protocol.hpp>
#include <lv/RPC/Common.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>

#include <boost/assert.hpp>
#include <boost/scope_exit.hpp>

#include <map>

#ifdef LV_PLATFORM_WINDOWS
#include <Windows.h>
#endif


namespace lv { namespace rpc {

	DEFINE_EXCEPTION_MSG(InvalidRequestID, std::runtime_error);

	/**
	 * thread-safe (as long as BufferManager and Socket passed in are thread-safe)
	 */
	template<class Id, class ArchivePair>
	class Client : public RpcBase
	{
	public:

		typedef boost::function<void(BufferPtr)>	callback_type;

	private:

		typedef typename ArchivePair::iarchive_type	iarchive_type;
		typedef typename ArchivePair::oarchive_type	oarchive_type;

		volatile long	next_request_id_;

		typedef typename Protocol::request_id_type request_id_type;

		typedef boost::shared_ptr<detail::PromiseBase<ArchivePair> >	PromiseBasePtr;
		typedef std::map<request_id_type, PromiseBasePtr>	promise_map;

		promise_map		promises_;

		typedef boost::mutex::scoped_lock scoped_lock;
		boost::mutex	mutex_;

		callback_type	callback_;

		template<typename Ret>
		class PrivateHandler
		{
			Client & client_;

			request_id_type	request_id_;

			BufferPtr	buffer_;

			bool	sent_;

		public:

			PrivateHandler(Client & client, request_id_type request_id, BufferPtr buf)
				: client_(client)
				, request_id_(request_id)
				, buffer_(buf)
				, sent_(false)
			{
			}

			/// copy constructor
			PrivateHandler(PrivateHandler & rhs)
				: client_(rhs.client_)
				, request_id_(rhs.request_id_)
				, buffer_(rhs.buffer_)
				, sent_(false)
			{
				BOOST_ASSERT(! rhs.sent_);
				rhs.sent_ = true;
			}

			~PrivateHandler()
			{
				if(! sent_)
				{
					client_.send(buffer_, request_id_, Protocol::options::none);
				}
			}

			operator Acknowledgment ()
			{
				sent_ = true;
				
				// if we call @a send first, @a on_receive might be called before 
				// @a add_promise is called and InvalidRequestID may be thrown
				boost::shared_ptr<detail::AchnowPromise<ArchivePair> > promise(new detail::AchnowPromise<ArchivePair>());
				client_.add_promise(request_id_, promise);

				try
				{
					client_.send(buffer_, request_id_, Protocol::options::ack);
				}
				catch(...)
				{
					client_.remove_promise(request_id_);
					throw;
				}

				return Acknowledgment(promise->get_future());		
			}

			operator ReturningHandler<Ret> ()
			{
				sent_ = true;
				//
				boost::shared_ptr<detail::ReturnPromise<Ret, ArchivePair> > promise(new detail::ReturnPromise<Ret, ArchivePair>());
				client_.add_promise(request_id_, promise);

				try
				{
					client_.send(buffer_, request_id_, Protocol::options::ret);
				}
				catch(...)
				{
					client_.remove_promise(request_id_);
					throw;
				}

				return ReturningHandler<Ret>(promise->get_future());
			}

			/**
			 * this will block until the result is sent back. It may be dangerous if the user 
			 *	doesn't notice that.
			 */
			operator Ret()
			{
				return ReturningHandler<Ret>(*this).get();
			}
		};

	public:

		Client(callback_type const & callback, BufferManagerPtr buf_manager)
			: RpcBase(buf_manager)
			, callback_(callback)
			, next_request_id_(1)
		{
		}

		virtual ~Client()
		{
		}


		/**
		 * @exception InvalidRequestID
		 * @exception InvalidProtocolValue
		 * @exception boost::archive::archive_exception
		 */
		void	on_receive(ConstBufferRef const & buf)
		{
			IArchiveCreator<iarchive_type> creator(istream_factory_, buf);
			iarchive_type & ia = creator.archive();

			Protocol::header::type header;
			ia >> header;
			
			if(header == Protocol::header::reply)
			{
				request_id_type id;
				ia >> id;

				// scoped lock
				scoped_lock lock(mutex_);
				
				promise_map::iterator it = promises_.find(id);
				if(it == promises_.end())
				{
					throw InvalidRequestID();
				}
				
				// 
				BOOST_SCOPE_EXIT((&promises_)(it))
				{
					promises_.erase(it);
				} BOOST_SCOPE_EXIT_END

				// the following code may throw exception
				it->second->set(ia);
			}
			else
			{
				throw InvalidProtocolValue("invalid Protocol::header value");
			}
			
		}

		/**
		 * call
		 */
#		define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_RPC_MAX_ARITY, <lv/RPC/Client.hpp>))
#		include BOOST_PP_ITERATE()


		// 
		void	set_exception(boost::exception_ptr ex)
		{
			scoped_lock lock(mutex_);

			promise_map pro;
			std::swap(pro, promises_);

			foreach(promise_map::reference v, pro)
			{
				try
				{
					v.second->set_exception(ex);
				}
				catch(boost::promise_already_satisfied const &)
				{
					BOOST_ASSERT(false);
				}
				catch(boost::broken_promise const &)
				{
					BOOST_ASSERT(false);
				}
			}
		}

	protected:

		virtual	void	send(BufferPtr buf)
		{
			callback_(buf);
		}
	
	private:

		class Serialize
		{
			oarchive_type & oa_;

		public:
			Serialize(oarchive_type & oa) : oa_(oa) {}

			template<typename T>
			void operator() (T const * t) const
			{
				// use RPC_REGISTER_CLASS to register your class
				//BOOST_STATIC_ASSERT((boost::serialization::implementation_level<T>::value != 
				//	boost::serialization::object_class_info));
				oa_ << *t;
			}
		};


		request_id_type	next_request_id()
		{
#ifdef LV_PLATFORM_WINDOWS

			return InterlockedIncrement(&next_request_id_);
#endif
		}

		template<typename Ret, class Tuple>
		PrivateHandler<Ret> call_impl(Id const & id, Tuple const & args)
		{
			BufferPtr buf = this->get_buffer();

			OStreamPtr raw_os = ostream_factory_.open(*buf);
			oarchive_type oa(*raw_os);

			oa << Protocol::header::call << id;
			boost::fusion::for_each(args, Serialize(oa));

			raw_os->flush();

			return PrivateHandler<Ret>(*this, next_request_id(), buf);
		}


		void	send(BufferPtr & buf, request_id_type request_id, typename Protocol::options::type call_option)
		{
			OStreamPtr raw_os = ostream_factory_.open(*buf);
			oarchive_type oa(*raw_os);

			oa << call_option;
			// sends the request id only when an acknowledgment or a return value is required
			if(call_option != Protocol::options::none)
			{
				oa << request_id;
			}

			raw_os->flush();

			send(buf);
		}

		/**
		 * @note the ownership of promise will be transfered to promises_
		 */
		void	add_promise(int request_id, PromiseBasePtr promise)
		{
			scoped_lock lock(mutex_);
			promises_.insert(std::make_pair(request_id, promise));
		}

		void	remove_promise(int request_id)
		{
			scoped_lock lock(mutex_);
			promises_.erase(request_id);
		}

	};

} }

#endif

#else

#define LV_RPC_call_params(z, n, data) T##n const & t##n
#define LV_RPC_pointer_type(z, n, data) T##n const *

#define n BOOST_PP_ITERATION()

template<typename Ret BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
PrivateHandler<Ret>	call(Id const & id BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, LV_RPC_call_params, ~))
{
	typedef boost::tuples::tuple<BOOST_PP_ENUM(n, LV_RPC_pointer_type, ~)> tuple_t;

#if n == 0
	tuple_t args;
#else
	tuple_t args(BOOST_PP_ENUM_PARAMS(n, &t));
#endif

	return this->call_impl<Ret>(id, args);
}


#undef LV_RPC_call_params
#undef LV_RPC_pointer_type
#undef n


#endif 