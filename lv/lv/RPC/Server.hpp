// *********************************************************************
//  Server   version:  1.0   ¡¤  date: 09/19/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_SERVER_HPP
#define LV_RPC_SERVER_HPP

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include <lv/rpc/Config.hpp>
#include <lv/rpc/Fwd.hpp>

#include <lv/rpc/Registery.hpp>
#include <lv/rpc/Invoker.hpp>
#include <lv/rpc/Exceptions.hpp>
#include <lv/rpc/Common.hpp>
#include <lv/rpc/RpcBase.hpp>
#include <lv/rpc/Protocol.hpp>
#include <lv/rpc/PacketArchive.hpp>

namespace lv { namespace rpc {

	/**
	 * This class is thread-safe as long as the BufferManager and Socket passed in are thread-safe
	 */
	template<class Reg>
	class Server : public RpcBase
	{
		typedef typename Reg::archive_pair_type archive_pair_type;
		typedef typename archive_pair_type::iarchive_type	iarchive_type;
		typedef typename archive_pair_type::oarchive_type	oarchive_type;

		typedef typename Reg::protocol_type	protocol_type;

		typedef typename protocol_type::request_id_type	request_id_type;

		std::auto_ptr<Reg> registery_;

		uint32	ex_seed_;
		uint32	func_seed_;
		
	public:

		/**
		 * @param buf_manager should be thread-safe
		 * @param registery use std::auto_ptr to indicate that the ownership of @a registery will be 
		 *	transfered and no functions should be registered any more.
		 */
		Server(BufferManagerPtr buf_manager, std::auto_ptr<Reg> registery)
			: RpcBase(buf_manager)
			, registery_(registery)
			, ex_seed_(exception_hash_seed<protocol_type::except_key_type>())
		{
			func_seed_ = this->registery_->hash();
		}

		uint32	exception_seed() const
		{
			return ex_seed_;
		}

		uint32	function_seed() const
		{
			return func_seed_;
		}

		void	on_connected(SocketPtr sock)
		{
			BufferPtr buf = this->get_buffer();

			namespace io = boost::iostreams;
			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_type oa(raw_os, boost::archive::no_header);

			oa << protocol_type::header::confer << ex_seed_ << func_seed_;
			raw_os.flush();

			this->send_buffer(buf, sock);
		}

		/**
		 * @param sock sock.send() should be thread-safe
		 * @exception InvalidFunctionID
		 * @exception InvalidProtocolValue
		 * @exception boost::archive::archive_exception
		 */
		template<class Range>
		void	on_receive(Range const & data, SocketPtr sock)
		{
			boost::iostreams::filtering_istream raw_is(boost::make_iterator_range(data));
			iarchive_type ia(raw_is);

			Reg::ExceptHolder	 ex;

			Reg::ResultHolder result;

			protocol_type::header::type header;
			ia >> header;
			if(header != protocol_type::header::call)
				throw InvalidProtocolValue("invalid Pro::header value");

			try
			{
				result = registery_->invoke(ia);
			}
			catch(InvalidProtocolValue const &)
			{
				throw;
			}
			catch(InvalidFunctionID const & )
			{
				throw;
			}
			catch(SerializationError const &)
			{
				throw;
			}
			catch(...)
			{
				ex = current_except<archive_pair_type, protocol_type>(ex_seed_);
			}

			protocol_type::options::type	call_option;
			ia >> call_option;

			if(call_option == protocol_type::options::none)
				return;
			else if(call_option != protocol_type::options::ret && call_option != protocol_type::options::ack)
				throw InvalidProtocolValue("invalid Pro::options value");

			// send back the result
			request_id_type id;
			ia >> id;
				
			BufferPtr buf = this->get_buffer();

			namespace io = boost::iostreams;
			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_type oa(raw_os, boost::archive::no_header);

			oa << protocol_type::header::reply << id;
			if(ex)
			{
				oa << protocol_type::except::has_ex;
				ex(oa);
			}
			else
			{
				oa << protocol_type::except::no_ex;
				if(call_option == protocol_type::options::ret)
					result(oa);	// write the result
			}

			raw_os.flush();

			this->send_buffer(buf, sock);
		}
	};

} }
#endif // LV_RPC_SERVER_HPP
