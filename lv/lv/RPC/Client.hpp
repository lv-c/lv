// *********************************************************************
//  Client   version:  1.0   ¡¤  date: 09/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_CLIENT_HPP
#define LV_RPC_CLIENT_HPP

#include <lv/Exception.hpp>
#include <lv/Ensure.hpp>

#include <lv/RPC/RpcBase.hpp>
#include <lv/RPC/Future.hpp>
#include <lv/RPC/Protocol.hpp>
#include <lv/RPC/Common.hpp>

#include <boost/assert.hpp>

#include <map>
#include <atomic>


namespace lv { namespace rpc {

	DEFINE_EXCEPTION_MSG(InvalidRequestID, std::runtime_error);

	/**
	 * thread-safe (as long as BufferManager and Socket passed in are thread-safe)
	 */
	template<class Id, class ArchivePair>
	class Client : public RpcBase
	{
	public:

		typedef std::function<void(BufferPtr)>	callback_type;

	private:

		typedef typename ArchivePair::iarchive_type	iarchive_type;
		typedef typename ArchivePair::oarchive_type	oarchive_type;

		typedef typename Protocol::request_id_type request_id_type;

		std::atomic<request_id_type>	next_request_id_;


		typedef std::shared_ptr<detail::IPromise<ArchivePair> >	PromiseBasePtr;
		typedef std::map<request_id_type, PromiseBasePtr>	promise_map;

		promise_map		promises_;

		typedef std::lock_guard<std::mutex>	lock_guard;

		std::mutex	mutex_;

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
				auto promise = pool::alloc<promise_type>();

				client_.add_promise(request_id_, promise);

				try
				{
					Protocol::options::type option = (boost::is_same<Ret, void>::value ? Protocol::options::ack : Protocol::options::ret);
					client_.send(buffer_, oa_, raw_os_, request_id_, option);
				}
				catch (...)
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
				lock_guard lock(mutex_);
				
				promise_map::iterator it = promises_.find(id);
				LV_ENSURE(it != promises_.end(), InvalidRequestID());

				promise = it->second;
				promises_.erase(it);
			}
			
			// the following code may throw exception
			promise->set(ia);
		}


		template<typename Ret, typename ...Types>
		PrivateHandler<Ret> call(Id const & id, Types const &... args)
		{
			BufferPtr buf = this->get_buffer();

			OStreamPtr raw_os = ostream_factory_.open(*buf);
			auto oa = pool::alloc<oarchive_type>(boost::ref(*raw_os));

			(*oa) << Protocol::header::call << id;
			int dummy[] = { 0, ((*oa << args), 0)... };

			raw_os->flush();

			return PrivateHandler<Ret>(*this, buf, oa, raw_os, next_request_id_++);
		}


		// 
		void	set_exception(std::exception_ptr ex)
		{
			promise_map pro;

			{
				lock_guard lock(mutex_);
				std::swap(pro, promises_);
			}

			for (promise_map::reference v : pro)
			{
				try
				{
					v.second->set_exception(ex);
				}
				catch (std::future_error const &)
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
			lock_guard lock(mutex_);
			promises_.insert(std::make_pair(request_id, promise));
		}

		void	remove_promise(int request_id)
		{
			lock_guard lock(mutex_);
			promises_.erase(request_id);
		}

	};

} }

#endif
