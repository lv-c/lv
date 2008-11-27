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

#include <lv/RPC/Invoker.hpp>
#include <lv/RPC/Exceptions.hpp>
#include <lv/RPC/ISocket.hpp>
#include <lv/RPC/IBufferManager.hpp>

namespace lv { namespace rpc {


	template<class Reg = Registery<> >
	class Server
	{
		typedef typename Reg::archive_pair_type archive_pair_type;
		typedef typename archive_pair_type::iarchive_t	iarchive_t;
		typedef typename archive_pair_type::oarchive_t	oarchive_t;

		typedef typename Reg::protocol_type	protocol_type;

		typedef int32 request_id_type;

		std::auto_ptr<Reg> registery_;

		typedef boost::shared_ptr<Exceptions<archive_pair_type, protocol_type> > except_ptr;
		except_ptr except_;


		ISocketPtr	socket_;

		IBufferManagerPtr	buf_manager_;

		
	public:

		/**
		 * @registery use std::auto_ptr to indicate that the ownership of @a registery will be 
		 *	transfered and no functions should be registered any more.
		 */
		Server(ISocketPtr socket, IBufferManagerPtr buf_manager, std::auto_ptr<Reg> registery, except_ptr except)
			: socket_(socket)
			, buf_manager_(buf_manager)
			, registery_(registery)
			, except_(except)
		{
		}


		/**
		 * @exception InvalidFunctionID
		 * @exception boost::archive::archive_exception
		 */
		template<class Range>
		void	input(Range const & buf)
		{
			namespace io = boost::iostreams;
			iarchive_t ia(io::filtering_istream(buf));

			std::auto_ptr<ExceptHolder>	 ex;

			Reg::ResultHolderPtr result;
			protocol_type::options::type	call_option;

			try
			{
				result = registery_->invoke(ia);
				ia >> call_option;
			}
			catch(InvalidFunctionID const & )
			{
				throw;
			}
			catch(boost::archive::archive_exception const &)
			{
				throw;
			}
			catch(...)
			{
				ex = current_except();
			}

			if(call_option == protocol_type::options::none)
				return;



			// send back the result
			request_id_type id;
			ia >> id;
				
			BufferPtr buf = buf_manager_->get();

			buf = preprocess(buf);
			BOOST_ASSERT(buf);

			namespace io = boost::iostreams;
			io::filtering_ostream raw_os(io::back_inserter(*buf));
			oarchive_t oa(raw_os, boost::archive::no_header);

			oa << id;
			if(ex.get() == NULL)
				oa << protocol_type::except::no_ex;
			else
			{
				//oa << protocol_type::except::has_ex << except_->
			}

		}

	protected:

		virtual	BufferPtr preprocess(BufferPtr buf) 
		{ 
			return socket_->preprocess(buf);
		}

		virtual	BufferPtr postprocess(BufferPtr buf) 
		{ 
			return buf; 
		}
	};

} }
#endif // LV_RPC_SERVER_HPP