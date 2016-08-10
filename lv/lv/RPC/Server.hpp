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

#include <lv/RPC/Registery.hpp>
#include <lv/RPC/Common.hpp>
#include <lv/RPC/RpcBase.hpp>
#include <lv/RPC/Protocol.hpp>
#include <lv/Ensure.hpp>


namespace lv { namespace rpc {

	/**
	 * This class is thread-safe as long as the BufferManager and Socket passed in are thread-safe
	 */
	template<class Id, class ArchivePair>
	class Server : public RpcBase
	{
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
		 * @return returns the reply buffer if there's
		 * @exception InvalidFunctionID
		 * @exception InvalidProtocolValue
		 * @exception boost::archive::archive_exception
		 */
		BufferPtr	on_receive(ConstBufferRef const & data)
		{
			IArchiveWrapper<iarchive_type> ia(istream_factory_, data);

			typename register_type::ResultHolder result;

			// TODO : call_option and id are better in the front, or they may be serialized as parameters

			Protocol::header::type header;
			ia.get() >> header;

			LV_ENSURE(header == Protocol::header::call, InvalidProtocolValue("invalid Pro::header value"));

			result = registery_.invoke(ia.get());

			Protocol::options::type	call_option;
			ia.get() >> call_option;

			if (call_option == Protocol::options::none)
			{
				return BufferPtr();
			}

			LV_ENSURE(call_option == Protocol::options::ret || call_option == Protocol::options::ack,
				InvalidProtocolValue("invalid Pro::options value"));

			// send back the result
			Protocol::request_id_type id;
			ia.get() >> id;

			BufferPtr buf = this->get_buffer();

			OArchiveWrapper<oarchive_type> oa(ostream_factory_, *buf);

			auto reply = Protocol::header::reply;
			oa.get() << reply << id;
			
			if (call_option == Protocol::options::ret)
			{
				result(oa.get());	// write the result
			}

			oa.flush();

			return buf;
		}

	};

} }

#endif
