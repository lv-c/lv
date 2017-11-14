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


namespace lv::rpc
{
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

		typedef	OArchiveWrapper<oarchive_type>	oarchive_wrapper;


		callback_type	callback_;

		typedef typename Protocol::request_id_type request_id_type;

		std::atomic<request_id_type>	next_request_id_;


		typedef std::shared_ptr<detail::IPromise<ArchivePair> >	PromiseBasePtr;
		typedef std::map<request_id_type, PromiseBasePtr>	promise_map;

		promise_map		promises_;

		typedef std::lock_guard<std::mutex>	lock_guard;

		std::mutex	mutex_;


		template<typename Ret>
		class PrivateHandler
		{
			Client & client_;

			BufferPtr	buffer_;

			std::unique_ptr<oarchive_wrapper>	oa_;

			request_id_type	request_id_;

			bool	sent_;

		public:

			PrivateHandler(Client & client, BufferPtr buf, std::unique_ptr<oarchive_wrapper> && oa, request_id_type request_id)
				: client_(client)
				, buffer_(buf)
				, oa_(std::move(oa))
				, request_id_(request_id)
				, sent_(false)
			{
			}

			PrivateHandler(PrivateHandler && rhs)
				: client_(rhs.client_)
				, buffer_(rhs.buffer_)
				, oa_(std::move(rhs.oa_))
				, request_id_(rhs.request_id_)
				, sent_(false)
			{
				BOOST_ASSERT(!rhs.sent_);
				rhs.sent_ = true;
			}

			~PrivateHandler()
			{
				if (!sent_)
				{
					client_.send(buffer_, *oa_, request_id_, Protocol::options::none);
				}
			}

			operator Future<Ret> ()
			{
				sent_ = true;
				
				// if we call @a send first, @a on_receive might be called before 
				// @a add_promise is called and InvalidRequestID may be thrown
				typedef detail::ReturnPromise<Ret, ArchivePair>	promise_type;
				auto promise = std::make_shared<promise_type>();

				client_.add_promise(request_id_, promise);

				try
				{
					Protocol::options::type option = (std::is_same_v<Ret, void> ? Protocol::options::ack : Protocol::options::ret);
					client_.send(buffer_, *oa_, request_id_, option);
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
			IArchiveWrapper<iarchive_type> ia(istream_factory_, buf);

			Protocol::header::type header;
			ia.get() >> header;
			
			LV_ENSURE(header == Protocol::header::reply, InvalidProtocolValue("invalid Protocol::header value"));

			request_id_type id;
			ia.get() >> id;

			// scoped lock
			PromiseBasePtr promise;

			{
				lock_guard lock(mutex_);
				
				auto it = promises_.find(id);
				LV_ENSURE(it != promises_.end(), InvalidRequestID());

				promise = it->second;
				promises_.erase(it);
			}
			
			// the following code may throw exception
			promise->set(ia.get());
		}


		template<typename Ret, typename ...Types>
		PrivateHandler<Ret> call(Id const & id, Types const &... args)
		{
			BufferPtr buf = this->get_buffer();
			std::unique_ptr<oarchive_wrapper> oa = std::make_unique<oarchive_wrapper>(ostream_factory_, *buf);

			auto header = Protocol::header::call;
			oa->get() << header << id;

			[[maybe_unused]] int dummy[] = { 0, ((oa->get() << args), 0)... };

			return PrivateHandler<Ret>(*this, buf, std::move(oa), next_request_id_++);
		}


		// 
		void	set_exception(std::exception_ptr ex)
		{
			promise_map pro;

			{
				lock_guard lock(mutex_);
				pro = std::move(promises_);
			}

			for (auto & v : pro)
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

		void	send(BufferPtr & buf, oarchive_wrapper & oa, request_id_type request_id, Protocol::options::type option)
		{
			oa.get() << option;
			// sends the request id only when an acknowledgment or a return value is required
			if (option != Protocol::options::none)
			{
				oa.get() << request_id;
			}

			oa.flush();

			send(buf, option);
		}

		/**
		 * @note the ownership of promise will be transfered to promises_
		 */
		void	add_promise(int request_id, PromiseBasePtr promise)
		{
			lock_guard lock(mutex_);
			promises_.emplace(request_id, promise);
		}

		void	remove_promise(int request_id)
		{
			lock_guard lock(mutex_);
			promises_.erase(request_id);
		}

	};

}

#endif
