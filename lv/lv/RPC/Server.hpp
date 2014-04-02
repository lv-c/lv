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

#include <lv/MemFn.hpp>

#include <lv/RPC/Config.hpp>
#include <lv/RPC/Registery.hpp>
#include <lv/RPC/Common.hpp>
#include <lv/RPC/RpcBase.hpp>
#include <lv/RPC/Protocol.hpp>

namespace lv { namespace rpc {

	/**
	 * This class is thread-safe as long as the BufferManager and Socket passed in are thread-safe
	 */
	template<class Id, class ArchivePair>
	class Server : public RpcBase
	{
	public:

		typedef boost::function<void(BufferPtr)>	callback_type;

	private:

		typedef typename ArchivePair::iarchive_type	iarchive_type;
		typedef typename ArchivePair::oarchive_type	oarchive_type;

		typedef detail::Registery<Id, ArchivePair>	register_type;
		register_type	registery_;

	public:

		/**
		 * @param buf_manager should be thread-safe
		 */
		Server(BufferManagerPtr buf_manager)
			: RpcBase(buf_manager)
		{
		}
		
		/**
		 * register a member function with a given object
		 * @param t could be an object, a pointer, a reference wrapper or 
		 *	a smart pointer.
		 */
		template<class MemFn, class T>
		Server &	reg_mem_fn(Id const & id, MemFn f, T t)
		{
			return reg<typename BindMemFnSignature<MemFn>::type>(id, bind_mem_fn(f, t));
		}

		/**
		 * register a function object
		 * @exception std::runtime_error if @a id has already been used
		 */
		template<class F>
		Server &	reg(Id const & id, F f)
		{
			return reg<typename Signature<F>::type, F>(id, f);
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a id has already been used
		 */
		template<class Signature, class F>
		Server &	reg(Id const & id, F f)
		{
			registery_.reg<Signature>(id, f);
			return *this;
		}


		/**
		 * @param sock sock.send() should be thread-safe
		 * @exception InvalidFunctionID
		 * @exception InvalidProtocolValue
		 * @exception boost::archive::archive_exception
		 */
		void	on_receive(ConstBufferRef const & data, callback_type const & callback)
		{
			IArchiveCreator<iarchive_type> creator(istream_factory_, data);
			iarchive_type & ia = creator.archive();

			register_type::ResultHolder result;

			// TODO : call_option and id are better in the front, or they may be serialized as parameters

			Protocol::header::type header;
			ia >> header;

			if(header != Protocol::header::call)
			{
				throw InvalidProtocolValue("invalid Pro::header value");
			}

			result = registery_.invoke(ia);

			Protocol::options::type	call_option;
			ia >> call_option;

			if(call_option == Protocol::options::none)
			{
				return;
			}
			else if(call_option != Protocol::options::ret && call_option != Protocol::options::ack)
			{
				throw InvalidProtocolValue("invalid Pro::options value");
			}

			// send back the result
			Protocol::request_id_type id;
			ia >> id;
				
			BufferPtr buf = this->get_buffer();

			OStreamPtr raw_os = ostream_factory_.open(*buf);
			oarchive_type oa(*raw_os);

			oa << Protocol::header::reply << id;
			
			if(call_option == Protocol::options::ret)
			{
				result(oa);	// write the result
			}

			raw_os->flush();

			callback(buf);
		}

	};

} }

#endif
