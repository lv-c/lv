// *********************************************************************
//  Registery   version:  1.0   ¡¤  date: 09/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_REGISTERY_HPP
#define LV_RPC_REGISTERY_HPP

#include <lv/RPC/Fwd.hpp>
#include <lv/RPC/Invoker.hpp>

#include <lv/Functional.hpp>
#include <lv/Exception.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <map>


namespace lv { namespace rpc {

	DEFINE_EXCEPTION_MSG(InvalidFunctionID, std::runtime_error);

	class SerializationError : public boost::archive::archive_exception
	{
		typedef boost::archive::archive_exception	base_type;

		std::string const msg_;

	public:

		SerializationError(boost::archive::archive_exception::exception_code code, std::string const & msg)
			: boost::archive::archive_exception(code)
			, msg_(base_type::what() + msg)
		{
		}

		virtual char const * what() const noexcept
		{
			return msg_.c_str();
		}
	};


	namespace detail
	{
		template<typename Id, class ArchivePair>
		class Registery : boost::noncopyable
		{
		public:

			typedef typename detail::InvokerBase<ArchivePair>::ResultHolder	ResultHolder;

			typedef ArchivePair		archive_pair_type;

		private:

			typedef typename ArchivePair::iarchive_type iarchive_type;

			typedef	Id				id_type;

			typedef boost::function<ResultHolder(iarchive_type &)>	invoker_type;

			typedef std::map<id_type, invoker_type>	invoker_map;
			invoker_map		invokers_;

			boost::shared_mutex	mutex_;

		public:

			/**
			 * @exception std::runtime_error if @a id has already been used
			 */
			template<class Signature, class F>
			Registery &	reg(Id const & id, F f)
			{
				boost::lock_guard<boost::shared_mutex> lock(mutex_);

				if (invokers_.find(id) != invokers_.end())
				{
					throw std::runtime_error("The id has already been used");
				}

				invokers_.insert(std::make_pair(id, detail::Invoker<Signature, ArchivePair>(f)));

				return *this;
			}

			/**
			 * @exception InvalidFunctionID no function found associated with the given id
			 * @exception SerializationError  ( public boost::archive::archive_exception)
			 * @exception exceptions thrown by the target function
			 */
			ResultHolder	invoke(iarchive_type & ia)
			{
				id_type id;
				ia >> id;

				invoker_map::iterator it;
				
				{
					// no deletion, so the iterator will be valid
					boost::shared_lock<boost::shared_mutex> lock(mutex_);
				
					it = invokers_.find(id);
					if (it == invokers_.end())
					{
						throw InvalidFunctionID("invalid function id: " + boost::lexical_cast<std::string>(id));
					}
				}

				try
				{
					return it->second(ia);
				}
				catch(boost::archive::archive_exception const & ex)
				{
					throw SerializationError(ex.code, ".error calling function:" + boost::lexical_cast<std::string>(id));
				}
				catch(...)
				{
					throw;
				}
			}

		};
	}
} }

#endif

