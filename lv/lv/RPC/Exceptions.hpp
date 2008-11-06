// *********************************************************************
//  Exceptions   version:  1.0   ¡¤  date: 09/22/2008
//  --------------------------------------------------------------------
//  Enable transporting of exceptions between client and server.
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_EXCEPTIONS_HPP
#define LV_RPC_EXCEPTIONS_HPP

#include <list>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <boost/future.hpp>
#include <boost/exception_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <lv/Algorithm/UniqueHash.hpp>
#include <lv/Exception.hpp>

#include <boost/exception/exception.hpp>

namespace lv { namespace rpc { 

	template<class ArchivePair>
	class ExceptIO
	{
	public:

		/**
		 * @exception boost::archive::archive_exception ?
		 */
		virtual	boost::exception_ptr get(typename ArchivePair::iarchive_t & ia) = 0;

		// All the subclasses should also define the following function to serialize the 
		// exception @a t to @a oa
		// static void write(typename ArchivePair:oarchive_t & oa, T t);
	};

	typedef boost::shared_ptr<ExceptIO>	ExceptIOPtr;




	/**
	 * default implementation. T is serializable.
	 */
	template<class T, class ArchivePair, class Enable = void>
	class ExceptIOImpl : public ExceptIO<ArchivePair>
	{
	public:

		virtual	boost::exception_ptr get(typename ArchivePair::iarchive_t & ia)
		{
			T t;
			ia >> t;
			return boost::copy_exception(t);
		}

		static void write(typename ArchivePair:oarchive_t & oa, T t)
		{
			oa << t;
		}
	};



	// exception holder
	template<class ArchivePair>
	class ExceptHolder
	{
		std::string	name_;

	public:
		ExceptHolder(std:;string const & name) : name_(name) {}

		virtual	void write(typename ArchivePair:oarchive_t & oa) = 0;

		std::string const & name() const
		{
			return this->name_;
		}
	};

	template<class T, class ArchivePair>
	class ExceptHolderImpl : public ExceptHolder<ArchivePair>
	{
		T	ex_;
	public:
		
		ExceptHolderImpl(T const & ex, std::string const & name)
			: ex_(ex)
			, ExceptHolder<ArchivePair>(name)
		{
		}

		virtual void write(typename ArchivePair:oarchive_t & oa)
		{
			ExceptIOImpl<T, ArchivePair>::write(oa, ex_);
		}
	};


	DEFINE_EXCEPTION_MSG(RpcUnknownException, std::exception);


	template<class ArchivePair>
	std::auto_ptr<ExceptHolder>	current_except()
	{
		try
		{
			throw;		// rethrow the current exception
		}
		// catch the registered exceptions
#define	RPC_REG_EXCEP(except)			\
	catch(except const & ex) { return std::auto_ptr<ExceptHolder>(new ExceptHolderImpl<except, ArchivePair>(ex, #except)) }	
#include <lv/RPC/RegisterExceptions.hpp>

		// catch unknown exceptions
		catch(...) 
		{
			return std::auto_ptr<ExceptHolder>(new ExceptHolderImpl<RpcUnknownException, ArchivePair>(RpcUnknownException(), "RpcUnknownException")) ;
		}
	}


	/**
	 * partial specialization for std::exception and all the exception classes derived from std::exception.
	 */
	template<class T, class ArchivePair>
	class ExceptIOImpl<T, ArchivePair, typename boost::enable_if<
		boost::is_base_and_derived<std::exception, T> >::type>
		: public ExceptIO<ArchivePair>
	{
	public:

		virtual	boost::exception_ptr get(typename ArchivePair::iarchive_t & ia)
		{
			std::string str;
			ia >> str;
			return boost::copy_exception(T(str.c_str()));
		}

		static void	write(typename ArchivePair:oarchive_t & oa, T const & ex)
		{
			oa << std::string(ex.what());
		}

	};


	uint32	exception_hash_seed()
	{
		std::list except_list;

#define	RPC_REG_EXCEP(ex) except_list.push_back(#ex);
#include <lv/RPC/RegisterExceptions.hpp>

		return unique_hash::seed(except_list);
	}

	// no exception is found associated with the specified key.
	DEFINE_EXCEPTION_MSG(ExceptionNotFound, std::runtime_error);
	// exception hash key from the remote server is not the same as the local one.
	DEFINE_EXCEPTION_MSG(InvalidHashSeed, std::runtime_error);

	/**
	 * It was derived from boost::noncopyable to prevent unintended copying.
	 * @param Key an exception will be mapped to a value of integral type @a Key 
	 *	using UniqueHash. Usually an int16 type should be competent. Otherwise you 
	 *	can use an int32 type.
	 */
	template<class ArchivePair, class Pro>
	class Exceptions : boost::noncopyable
	{
		typedef typename Pro::except_key_type key_type;

		typedef boost::unordered_map<key_type, ExceptIOPtr>	except_map;

		except_map except_;

		int32	seed_;

	public:

		/**
		 * @param seed hash seed for registered exception types of the server.It SHOULD be the same
		 *	as the hash seed for registered exception types of the client.
		 * @exception InvalidHashSeed if(seed != exception_hash_seed())
		 */
		Exceptions(int32 seed)
			: seed_(seed)
		{
			if(seed != exception_hash_seed())
				throw InvalidHashSeed();

#define RPC_REG_EXCEP(ex) except_.insert(std::make_pair(unique_hash::hash(seed_, #ex), \
	ExceptIOPtr(new ExceptIOImpl<ex, ArchivePair>())));
#include <lv/RPC/RegisterExceptions.hpp>

		}

		/**
		 * number of exception types registered.
		 */
		size_t	size() const
		{
			return except_.size();
		}

		/**
		 * @exception ExceptionNotFound no exception found associated with @key.
		 * @exception boost::archive::archive_exception ? error serializing (loading) the exception.
		 */
		virtual	boost::exception_ptr get(key_type key, typename ArchivePair::iarchive_t & ia)
		{
			except_map::iterator it = except_.find(key);
			if(it == except_.end())
				throw ExceptionNotFound();
			else
				return it->second.get(ia);
		}
	};


} } 


#endif // LV_RPC_EXCEPTIONS_HPP