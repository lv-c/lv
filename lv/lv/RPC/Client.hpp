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
#include <lv/Ensure.hpp>

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
#include <boost/typeof/typeof.hpp>

#include <map>

#ifdef LV_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
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

		typedef std::shared_ptr<detail::IPromise<ArchivePair> >	PromiseBasePtr;
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

			std::shared_ptr<oarchive_type>	oa_;

			OStreamPtr	raw_os_;

			bool	sent_;

		public:

			PrivateHandler(Client & client, BufferPtr buf, std::shared_ptr<oarchive_type> oa, OStreamPtr raw_os, request_id_type request_id)
				: client_(client)
				, buffer_(buf)
				, oa_(oa)
				, raw_os_(raw_os)
				, request_id_(request_id)
				, sent_(false)
			{
			}

			/// copy constructor
			PrivateHandler(PrivateHandler & rhs)
				: client_(rhs.client_)
				, buffer_(rhs.buffer_)
				, oa_(rhs.oa_)
				, raw_os_(rhs.raw_os_)
				, request_id_(rhs.request_id_)
				, sent_(false)
			{
				BOOST_ASSERT(! rhs.sent_);
				rhs.sent_ = true;
			}

			~PrivateHandler()
			{
				if (! sent_)
				{
					client_.send(buffer_, oa_, raw_os_, request_id_, Protocol::options::none);
				}
			}

			operator Future<Ret> ()
			{
				sent_ = true;
				
				// if we call @a send first, @a on_receive might be called before 
				// @a add_promise is called and InvalidRequestID may be thrown
				typedef detail::ReturnPromise<Ret, ArchivePair>	promise_type;
				BOOST_AUTO(promise, pool::alloc<promise_type>());

				client_.add_promise(request_id_, promise);

				try
				{
					Protocol::options::type option = (boost::is_same<Ret, void>::value ? Protocol::options::ack : Protocol::options::ret);
					client_.send(buffer_, oa_, raw_os_, request_id_, option);
				}
				catch(...)
				{
					client_.remove_promise(request_id_);
					throw;
				}

				return promise->get_future();
			}

			/**
			 * this will block until the result is sent back. It may be dangerous if the user 
			 *	doesn't notice that.
			 */
			operator Ret()
			{
				return Future<Ret>(*this).get();
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
			
			LV_ENSURE(header == Protocol::header::reply, InvalidProtocolValue("invalid Protocol::header value"));

			request_id_type id;
			ia >> id;

			// scoped lock
			PromiseBasePtr promise;

			{
				scoped_lock lock(mutex_);
				
				promise_map::iterator it = promises_.find(id);
				LV_ENSURE(it != promises_.end(), InvalidRequestID());

				promise = it->second;
				promises_.erase(it);
			}
			
			// the following code may throw exception
			promise->set(ia);
		}

		/**
		 * call
		 */
#		define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_RPC_MAX_ARITY, <lv/RPC/Client.hpp>))
#		include BOOST_PP_ITERATE()


		// 
		void	set_exception(boost::exception_ptr ex)
		{
			promise_map pro;

			{
				scoped_lock lock(mutex_);
				std::swap(pro, promises_);
			}

			BOOST_FOREACH(promise_map::reference v, pro)
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

		virtual	void	send(BufferPtr buf, Protocol::options::type option)
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
			BOOST_AUTO(oa, pool::alloc<oarchive_type>(boost::ref(*raw_os)));

			(*oa) << Protocol::header::call << id;
			boost::fusion::for_each(args, Serialize(*oa));

			raw_os->flush();

			return PrivateHandler<Ret>(*this, buf, oa, raw_os, next_request_id());
		}


		void	send(BufferPtr & buf, std::shared_ptr<oarchive_type> oa, OStreamPtr raw_os, request_id_type request_id, Protocol::options::type option)
		{
			(*oa) << option;
			// sends the request id only when an acknowledgment or a return value is required
			if (option != Protocol::options::none)
			{
				(*oa) << request_id;
			}

			raw_os->flush();

			send(buf, option);
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