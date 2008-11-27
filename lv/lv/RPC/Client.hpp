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
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/noncopyable.hpp>

#include <lv/IntType.hpp>
#include <lv/RPC/Config.hpp>
#include <lv/RPC/ISocket.hpp>
#include <lv/RPC/IBufferManager.hpp>

#include <lv/RPC/Future.hpp>
#include <lv/RPC/PacketArchive.hpp>
#include <lv/RPC/Protocol.hpp>
#include <lv/RPC/Fwd.hpp>

namespace lv { namespace rpc {


	struct PacketArchive;
	struct Protocol;

	template<typename Id = std::string, class ArchivePair = PacketArchive, class Pro = Protocol>
	class Client : boost::noncopyable
	{

		typedef typename ArchivePair::iarchive_t	iarchive_t;
		typedef typename ArchivePair::oarchive_t	oarchive_t;

		ISocketPtr	socket_;

		IBufferManagerPtr	buf_manager_;

		typedef int32 request_id_type;
		request_id_type		next_request_id_;

		typedef std::auto_ptr<detail::PromiseBase<ArchivePair> > promise_ptr;

		typedef boost::ptr_unordered_map<int32, detail::PromiseBase<ArchivePair>, boost::hash<int32>, std::equal_to<int32>,
			boost::heap_clone_allocator, boost::pool_allocator<std::pair<int32, void*> > >	promise_map;

		promise_map	promises_;

		typedef boost::shared_ptr<Exceptions<ArchivePair, Pro> > except_ptr;
		except_ptr except_;


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
				//
				client_.send(buffer_, request_id_, Pro::options::ack);

				detail::AchnowPromise<ArchivePair, Pro> * promise = new detail::AchnowPromise<ArchivePair, Pro>(*client_.except_);
				client_.add_promise(request_id_, promise_ptr(promise));

				return Acknowledgment(*promise);		
			}

			operator ReturningHandler<Ret> ()
			{
				sent_ = true;
				//
				client_.send(buffer_, request_id_, Pro::options::ret);

				detail::ReturnPromise<Ret, ArchivePair, Pro> * promise = new detail::ReturnPromise<Ret, ArchivePair, Pro>(*client_.except_);
				client_.add_promise(request_id_, promise_ptr(promise));

				return ReturningHandler<Ret>(*promise);
			}
		};

	public:

		Client(ISocketPtr socket, IBufferManagerPtr buf_manager, except_ptr except)
			: socket_(socket)
			, buf_manager_(buf_manager)
			, except_(except)
			, next_request_id_(1)
		{
		}


		void on_receive(ConstBufferRef buf)
		{
			
		}


		bool connected() const
		{
			return socket_->connected();
		}

		/**
		 * calls a remote function.
		 */
#		define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_RPC_MAX_ARITY, <lv/RPC/Client.hpp>))
#		include BOOST_PP_ITERATE()


	protected:

		virtual	BufferPtr preprocess(Id const & id, BufferPtr buf) 
		{ 
			return socket_->preprocess(buf);
		}

		virtual	BufferPtr postprocess(BufferPtr buf) 
		{ 
			return buf; 
		}

	private:

		class Serialize
		{
			oarchive_t & oa_;

		public:
			Serialize(oarchive_t & oa) : oa_(oa) {}

			template<typename T>
			void operator() (T const * t) const
			{
				oa_ << *t;
			}
		};

		template<typename Ret, class Tuple>
		PrivateHandler<Ret> call_impl(Id const & id, Tuple const & args)
		{
			BufferPtr buf = buf_manager_->get();

			buf = preprocess(id, buf);
			BOOST_ASSERT(buf);

			namespace io = boost::iostreams;
			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_t oa(raw_os, boost::archive::no_header);

			oa << Pro::header::call << id;
			boost::fusion::for_each(args, Serialize(oa));

			return PrivateHandler<Ret>(*this, next_request_id_ ++, buf);
		}


		void	send(BufferPtr & buf, request_id_type request_id, typename Pro::options::type call_option)
		{
			namespace io = boost::iostreams;

			{	// auto flush
				io::filtering_ostream raw_os(io::back_inserter(*buf));
				oarchive_t oa(raw_os, boost::archive::no_header);

				oa << call_option;
				// sends the request id only when an acknowledgment or a return value is required
				if(call_option != Pro::options::none)
					oa << request_id;
			}

			socket_->send(postprocess(buf));
		}

		/**
		 * @note the ownership of promise will be transfered to promises_
		 */
		inline void	add_promise(int request_id, promise_ptr promise)
		{
			promises_.insert(request_id, promise);
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