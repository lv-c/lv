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

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>

#include <boost/assert.hpp>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <lv/IntType.hpp>
#include <lv/Exception.hpp>

#include <lv/rpc/Config.hpp>
#include <lv/rpc/Fwd.hpp>

#include <lv/rpc/RpcBase.hpp>
#include <lv/rpc/Future.hpp>
#include <lv/rpc/PacketArchive.hpp>
#include <lv/rpc/Protocol.hpp>
#include <lv/rpc/Exceptions.hpp>

namespace lv { namespace rpc {


	struct PacketArchive;
	struct Protocol;

	DEFINE_EXCEPTION_MSG(InvalidRequestID, std::runtime_error);

	// exception hash seed from the remote server is not the same as the local one.
	// (maybe the client is an older version and the user should update it)
	DEFINE_EXCEPTION_MSG(UnmatchedExceptSeed, std::runtime_error);

	/**
	 * thread-safe (as long as BufferManager and Socket passed in are thread-safe)
	 */
	template<typename Id, class ArchivePair, class Pro>
	class Client : public RpcBase
	{

		typedef typename ArchivePair::iarchive_t	iarchive_t;
		typedef typename ArchivePair::oarchive_t	oarchive_t;

		SocketPtr	socket_;

		typedef typename Pro::request_id_type request_id_type;
		request_id_type		next_request_id_;


		typedef boost::function<void(iarchive_t &)>	promise_base;
		typedef boost::unordered_map<request_id_type, 
			promise_base, 
			boost::hash<request_id_type>, 
			std::equal_to<request_id_type>, 
			boost::pool_allocator<std::pair<request_id_type, promise_base> >
		>	promise_map;

		promise_map	promises_;

		typedef boost::shared_ptr<Exceptions<ArchivePair, Pro> > except_ptr;
		except_ptr except_;

		typedef boost::mutex::scoped_lock scoped_lock;
		boost::mutex	mutex_;

		uint32	func_seed_;

		bool	ready_;

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
					client_.send(buffer_, request_id_, Pro::options::none);
			}

			operator Acknowledgment ()
			{
				sent_ = true;
				
				// if we call @a send first, @a on_receive might be called before 
				// @a add_promise is called and InvalidRequestID may be thrown
				detail::AchnowPromise<ArchivePair, Pro> promise(*client_.except_);
				client_.add_promise(request_id_, promise);

				try
				{
					client_.send(buffer_, request_id_, Pro::options::ack);
				}
				catch (...)
				{
					client_.remove_promise(request_id_);
					throw;
				}

				return Acknowledgment(promise);		
			}

			operator ReturningHandler<Ret> ()
			{
				sent_ = true;
				//
				detail::ReturnPromise<Ret, ArchivePair, Pro> promise(*client_.except_);
				client_.add_promise(request_id_, promise);

				try
				{
					client_.send(buffer_, request_id_, Pro::options::ret);
				}
				catch (...)
				{
					client_.remove_promise(request_id_);
					throw;
				}

				return ReturningHandler<Ret>(promise);
			}

			/**
			 * this will block until the result is sent back. It may be dangerous if the user 
			 *	doesn't notice that.
			 */
			operator Ret()
			{
				return ReturningHandler<Ret>(*this);
			}
		};

	public:

		Client(SocketPtr socket, BufferManagerPtr buf_manager, except_ptr except)
			: RpcBase(buf_manager)
			, socket_(socket)
			, except_(except)
			, next_request_id_(1)
			, ready_(false)
		{
		}


		/**
		 * @exception InvalidRequestID
		 * @exception InvalidExceptionID
		 * @exception UnmatchedExceptSeed
		 * @exception InvalidProtocolValue
		 * @exception boost::archive::archive_exception
		 */
		template<class Range>
		void	on_receive(Range const & buf)
		{
			boost::iostreams::filtering_istream raw_is(boost::make_iterator_range(buf));
			iarchive_t ia(raw_is);

			Pro::header::type header;
			ia >> header;
			
			if(header == Pro::header::reply)
			{
				request_id_type id;
				ia >> id;

				// scoped lock
				scoped_lock lock(mutex_);
				
				promise_map::iterator it = promises_.find(id);
				if(it == promises_.end())
					throw InvalidRequestID();
				
				try
				{
					it->second(ia);
				}
				catch (...)
				{
					promises_.erase(it);
					throw;
				}
			}
			else if(header == Pro::header::confer)
			{
				scoped_lock lock(mutex_);

				uint32 ex_seed;
				ia >> ex_seed >> func_seed_;
				if(ex_seed != except_->seed())
					throw UnmatchedExceptSeed();

				ready_ = true;
			}
			else
				throw InvalidProtocolValue("invalid Pro::header value");
			
		}
		
		/**
		 * returns true when the exception seed and the function id seed are received from 
		 *	the server or set by the user and you can invoke @call now.
		 */
		bool ready()
		{
			scoped_lock lock(mutex_);
			return ready_;
		}

		/**
		 * if you are quite sure what these seeds are, you can set it directly rather than 
		 *	receiving them from the server
		 * @exception UnmatchedExceptSeed
		 */
		void	set_seeds(uint32	ex_seed, uint32	func_seed)
		{
			scoped_lock lock(mutex_);

			if(ex_seed != except_->seed())
				throw UnmatchedExceptSeed();

			this->func_seed_ = func_seed;
			ready_ = true;
		}

		SocketPtr	socket() const
		{
			return socket_;
		}


		/**
		 * calls a remote function. Only call this when ready() returns true
		 * @exception std::runtime_error if ready() returns false
		 */
#		define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_RPC_MAX_ARITY, <lv/rpc/Client.hpp>))
#		include BOOST_PP_ITERATE()

	
	private:

		class Serialize
		{
			oarchive_t & oa_;

		public:
			Serialize(oarchive_t & oa) : oa_(oa) {}

			template<typename T>
			void operator() (T const * t) const
			{
				// use RPC_REGISTER_CLASS to register your class
				//BOOST_STATIC_ASSERT((boost::serialization::implementation_level<T>::value != 
				//	boost::serialization::object_class_info));
				oa_ << *t;
			}
		};

		template<typename Ret, class Tuple>
		PrivateHandler<Ret> call_impl(Id const & id, Tuple const & args)
		{
			BufferPtr buf = this->get_buffer();

			namespace io = boost::iostreams;
			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_t oa(raw_os, boost::archive::no_header);

			oa << Pro::header::call << unique_hash::hash<typename Pro::id_key_type const>(func_seed_, id);
			boost::fusion::for_each(args, Serialize(oa));

			return PrivateHandler<Ret>(*this, next_request_id_ ++, buf);
		}


		void	send(BufferPtr & buf, request_id_type request_id, typename Pro::options::type call_option)
		{
			namespace io = boost::iostreams;

			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_t oa(raw_os, boost::archive::no_header);

			oa << call_option;
			// sends the request id only when an acknowledgment or a return value is required
			if(call_option != Pro::options::none)
				oa << request_id;

			raw_os.flush();

			this->send_buffer(buf, socket_);
		}

		/**
		 * @note the ownership of promise will be transfered to promises_
		 */
		void	add_promise(int request_id, promise_base promise)
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

#endif // LV_RPC_CLIENT_HPP

#else

#define LV_RPC_call_params(z, n, data) T##n const & t##n
#define LV_RPC_pointer_type(z, n, data) T##n const *

#define n BOOST_PP_ITERATION()

template<typename Ret BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
PrivateHandler<Ret>	call(Id const & id BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, LV_RPC_call_params, ~))
{
	if(! ready())
		throw(std::runtime_error("It's not ready yet!"));

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