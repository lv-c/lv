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

#include <list>
#include <map>

#include <boost/fusion/container/map.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/noncopyable.hpp>

#include <lv/Functional.hpp>
#include <lv/Algorithm/UniqueHash.hpp>
#include <lv/Foreach.hpp>
#include <lv/Exception.hpp>

#include <lv/rpc/Fwd.hpp>
#include <lv/rpc/Invoker.hpp>
#include <lv/rpc/Exceptions.hpp>


namespace lv { namespace rpc {

	struct PacketArchive;
	struct Protocol;

	DEFINE_EXCEPTION_MSG(InvalidFunctionID, std::runtime_error);


	class SerializationError : public boost::archive::archive_exception
	{
		typedef boost::archive::archive_exception	base_type;

		std::string const msg_;
	public:

		SerializationError(boost::archive::archive_exception::exception_code code, std::string const & msg)
			: boost::archive::archive_exception(code)
			, msg_(std::string(base_type::what()) + msg)
		{
		}

		virtual char const * what() const
		{
			return msg_.c_str();
		}
	};


	template<class ParamExtractors, typename Id, class ArchivePair, class Pro>
	class Registery : boost::noncopyable
	{

		typedef typename ArchivePair::iarchive_type iarchive_type;
		typedef typename ArchivePair::oarchive_type oarchive_type;

		typedef typename detail::InvokerBase<ArchivePair>::ResultHolder	ResultHolder;
		typedef typename ExceptHolder<oarchive_type>::type	ExceptHolder;


		typedef typename Pro::id_key_type id_key_type;

		typedef boost::function<ResultHolder(iarchive_type &, ParamExtractors)>	invoker_type;

		typedef boost::unordered_map<id_key_type, invoker_type> hash_invoker_map;
		hash_invoker_map	hash_invoker_;

		// used to look up the function id when failed to serialize the parameter
		typedef boost::unordered_map<id_key_type, Id>	hash_id_map;
		hash_id_map	hash_id_;

		typedef std::map<Id, invoker_type>	id_invoker_map;
		id_invoker_map		id_invoker_;


		ParamExtractors	extractors_;

	private:

		template<class> friend class Server;

		typedef	Id				id_type;
		typedef ArchivePair		archive_pair_type;
		typedef	Pro				protocol_type;


		/** 
		 * 
		 * This should be called after all the functions have been registered and should only be called once.
		 * @exception NoSeedFoundError if there's no seed found with which the unique_hash::hash function will
		 *	generate a unique hash code for each id in @a id_invoker_. You should change Pro::id_key_type to 
		 *	a larger integral type.
		 * @exception std::logic_error this function has already been called (more precisely, hash_invoker_ is not empty)
		 */
		uint32	hash()
		{
			if(! hash_invoker_.empty())
				throw std::logic_error("hash() functions should only be called once.");

			// compute the hash seed
			std::list<Id> ids;
			foreach(id_invoker_map::value_type & item, id_invoker_)
			{
				ids.push_back(item.first);
			}
			uint32 seed = unique_hash::seed<id_key_type>(ids);

			// hash
			foreach(id_invoker_map::value_type & item, id_invoker_)
			{
				id_key_type key = unique_hash::hash<id_key_type>(seed, item.first);
				hash_invoker_.insert(std::make_pair(key, item.second));
				hash_id_.insert(std::make_pair(key, item.first));
			}

			// clear id_invoker_ to save memory 
			id_invoker_.clear();

			return seed;
		}

		/**
		 * @exception InvalidFunctionID no function found associated with the given id
		 * @exception SerializationError  ( public boost::archive::archive_exception)
		 * @exception exceptions thrown by the target function
		 */
		ResultHolder	invoke(typename ArchivePair::iarchive_type & ia)
		{
			id_key_type id;
			ia >> id;

			hash_invoker_map::iterator it = hash_invoker_.find(id);
			if(it == hash_invoker_.end())
				throw InvalidFunctionID(std::string("invalid function id: ") + boost::lexical_cast<std::string>(id));

			try
			{
				return it->second(ia, extractors_);
			}
			catch (boost::archive::archive_exception const & ex)
			{
				throw SerializationError(ex.code, std::string(".error calling function:") + boost::lexical_cast<std::string>(hash_id_[id]));
			}
			catch(...)
			{
				throw;
			}
		}


	public:

		explicit Registery(ParamExtractors const & extractors = ParamExtractors())
			: extractors_(extractors)
		{
		}


		/**
		 * register a function object
		 * @exception std::runtime_error if @a id has already been used
		 */
		template<class F>
		Registery & reg(Id const & id, F f)
		{
			return reg<typename Signature<F>::type, F>(id, f);
		}

		/**
		 * the signature is required to be explicitly pointed out and you can use this 
		 * function to register an overloaded function object
		 * @exception std::runtime_error if @a id has already been used
		 */
		template<class Signature, class F>
		Registery & reg(Id const & id, F f)
		{
			BOOST_ASSERT(hash_invoker_.empty() && "You should not register functions any more");

			if(id_invoker_.find(id) != id_invoker_.end())
				throw std::runtime_error("The id has already been used");

			id_invoker_.insert(std::make_pair(id, detail::Invoker<Signature, ArchivePair, ParamExtractors>(f)));

			return *this;
		}

	};

} }

#endif // LV_RPC_REGISTERY_HPP

