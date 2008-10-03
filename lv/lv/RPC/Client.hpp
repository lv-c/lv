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

#include <lv/IntType.hpp>
#include <lv/RPC/Config.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/noncopyable.hpp>

#include <lv/RPC/ISocket.hpp>
#include <lv/RPC/IBufferManager.hpp>

#include <lv/RPC/Future.hpp>

namespace lv { namespace rpc {


	struct PacketArchive;
	struct Protocol;

	template<typename Id = std::string, class ArchivePair = PacketArchive, class Pro = Protocol>
	class Client : boost::noncopyable
	{

		typedef typename ArchivePair::iarchive_t	iarchive_t;
		typedef typename ArchivePair::oarchive_t	oarchive_t;

		boost::shared_ptr<ISocket>	socket_;

		boost::shared_ptr<IBufferManager>	buf_manager_;

		int32		next_request_id_;

		typedef detail::PromiseBase * promise_ptr;

		typedef boost::ptr_unordered_map<int32, detail::PromiseBase, boost::hash<int32>, std::equal_to<int32>,
			boost::heap_clone_allocator, boost::pool_allocator<std::pair<int32, void*> > >	promise_map;

		promise_map	promises_;

		typedef boost::shared_ptr<Exceptions<ArchivePair, Pro> > except_ptr;
		except_ptr except_;


		template<typename Ret>
		class PrivateHandler
		{
			Client & client_;

			int	request_id_;

			BufferPtr	buffer_;

		public:
			PrivateHandler(Client & client, int request_id, BufferPtr buf)
				: client_(client)
				, request_id_(request_id)
				, buffer_(buf)
			{
			}

			~PrivateHandler()
			{
				client_.send(buffer_, request_id_, Pro::options::none);
			}

			operator Acknowledgment ()
			{
				client_.send(buffer_, request_id_, Pro::options::ack);

				detail::AchnowPromise<ArchivePair, Pro> * promise = new detail::AchnowPromise<ArchivePair, Pro>(*except_);
				client_.add_promise(request_id_, promise);

				return Acknowledgment(*promise);		
			}

			operator ReturningHandler<Ret> ()
			{
				client_.send(buffer_, request_id_, Pro::options::ret);

				detail::ReturnPromise<Ret, ArchivePair, Pro> * promise = new detail::ReturnPromise<Ret, ArchivePair, Pro>(*except_);
				client_.add_promise(request_id_, promise);

				return ReturningHandler<Ret>(*promise);
			}
		};

	public:

		Client(boost::shared_ptr<ISocket> socket, boost::shared_ptr<IBufferManager> buf_manager, except_ptr except)
			: socket_(socket)
			, buf_manager_(buf_manager)
			, except_(except)
			, next_request_id_(1)
		{
		}


		bool connected() const
		{
			return socket_->connected();
		}

		/**
		 * calls a remote function.
		 */
#		define BOOST_PP_ITERATION_PARAMS_1(3, (0, LV_RPC_MAX_ARITY, <lv/RPC/Client.hpp>))
#		include BOOST_PP_ITERATE()


	protected:

		virtual	BufferPtr preprocess(Id const & id, BufferPtr buf) 
		{ 
			return socket_->preprocess(buf);
		}

		virtual	BufferPtr postprocess(Id const & id, BufferPtr buf) 
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

			buf = preprocess(buf);
			BOOST_ASSERT(buf);

			namespace io = boost::iostreams;
			oarchive_t oa(io::filtering_ostream(io::back_inserter(*buf)), boost::archive::no_header);

			int32 request_id = next_request_id_++;
			oa << Pro::header::call << id;
			boost::fusion::for_each(args, Serialize(oa));
		}


		void	send(BufferPtr & buf, int request_id, typename Pro::options::type call_option)
		{
			namespace io = boost::iostreams;
			oarchive_t oa(io::filtering_ostream(io::back_inserter(*buf)), boost::archive::no_header);

			oa << call_option;
			// sends the request id only when an acknowledgment or a return value is required
			if(call_option != Pro::options::none)
				oa << request_id;

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

template<typename Ret BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, T)>
PrivateHandler<Ret>	call(Id const & id, BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, LV_RPC_call_params, ~))
{
	typedef boost::tuples::tuple<BOOST_PP_ENUM(n, LV_RPC_pointer_type, ~)> tuple_t;

#if n == 0
	tuple_t args;
#else
	tuple_t args(BOOST_PP_ENUM_PARAMS(n, &T));
#endif

	return this->call_impl<Ret>(id, args);
}


#undef LV_RPC_call_params
#undef LV_RPC_pointer_type
#undef n


#endif 