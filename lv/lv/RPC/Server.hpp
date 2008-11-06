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

namespace lv { namespace rpc {


	template<class Reg = Registery<> >
	class Server
	{
		typedef typename Reg::archive_pair_type archive_pair_type;
		typedef typename archive_pair_type::iarchive_t	iarchive_t;
		typedef typename archive_pair_type::oarchive_t	oarchive_t;

		typedef typename Reg::param_extractor_type	param_extractor_type;
		typedef typename Reg::protocol_type	protocol_type;

		std::auto_ptr<Reg> registery_;

		typedef boost::shared_ptr<Exceptions<archive_pair_type, protocol_type> > except_ptr;
		except_ptr except_;


		
	public:

		/**
		 * @registery use std::auto_ptr to indicate that the ownership of @a registery will be 
		 *	transfered and no functions should be registered any more.
		 */
		Server(std::auto_ptr<Reg> registery, except_ptr except)
			: registery_(registery)
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

			typedef detail::Invoker<archive_pair_type, param_extractor_type>	invoker_type;

			std::auto_ptr<typename invoker_type::ResultHolder> result;
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


			// send back the result

		}
	};

} }
#endif // LV_RPC_SERVER_HPP