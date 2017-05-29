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

#include <lv/Exception.hpp>
#include <lv/MPL/Functional.hpp>
#include <lv/Concurrent/SpinMutex.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/noncopyable.hpp>

#include <map>
#include <mutex>


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

		char const * what() const noexcept override
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

			typedef std::function<ResultHolder(iarchive_type &)>	invoker_type;

			typedef std::map<id_type, invoker_type>	invoker_map;
			invoker_map		invokers_;

			SpinMutex		mutex_;

		public:

			/**
			 * @exception std::runtime_error if @a id has already been used
			 */
			template<class Signature, class F>
			Registery &	reg(Id const & id, F f)
			{
				std::lock_guard<SpinMutex> lock(mutex_);

				if (invokers_.find(id) != invokers_.end())
				{
					throw std::runtime_error("The id has already been used");
				}

				invokers_.emplace(id, detail::Invoker<Signature, ArchivePair>(f));

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

				typename invoker_map::iterator it;
				
				{
					// no deletion, so the iterator will be valid
					std::lock_guard<SpinMutex> lock(mutex_);
				
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
				catch (boost::archive::archive_exception const & ex)
				{
					throw SerializationError(ex.code, ".error calling function:" + boost::lexical_cast<std::string>(id));
				}
				catch (...)
				{
					throw;
				}
			}

		};
	}
} }

#endif

