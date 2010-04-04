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
#include <lv/SharedPtr.hpp>

#include <lv/rpc/Fwd.hpp>

#include <boost/exception/exception.hpp>

namespace lv { namespace rpc { 

	template<class ArchivePair>
	class ExceptIO
	{
	protected:

		typedef typename ArchivePair::iarchive_type	iarchive_type;
		typedef typename ArchivePair::oarchive_type	oarchive_type;

	public:
		virtual	~ExceptIO() {}

		/**
		 * @exception boost::archive::archive_exception ?
		 */
		virtual	boost::exception_ptr get(iarchive_type & ia) = 0;

		// All the subclasses should also define the following function to serialize the 
		// exception @a t to @a oa
		// static void write(typename ArchivePair:iarchive_type & oa, T t);
	};

	template<class ArchivePair>
	struct ExceptIOPtr
	{
		typedef boost::shared_ptr<ExceptIO<ArchivePair> >	type;
	};

	/**
	 * default implementation. T is serializable.
	 */
	template<class T, class ArchivePair, class Enable = void>
	class ExceptIOImpl : public ExceptIO<ArchivePair>
	{
	public:

		virtual	boost::exception_ptr get(iarchive_type & ia)
		{
			T t;
			ia >> t;
			return boost::copy_exception(t);
		}

		static void write(oarchive_type & oa, T t)
		{
			oa << t;
		}
	};

	/**
	 * partial specialization for std::exception and all the exception classes derived from std::exception.
	 */
	template<class T, class ArchivePair>
	class ExceptIOImpl<T, ArchivePair, typename boost::enable_if<
		boost::is_base_and_derived<std::exception, T> >::type>
		: public ExceptIO<ArchivePair>
	{
	public:

		virtual	boost::exception_ptr get(iarchive_type & ia)
		{
			std::string str;
			ia >> str;
			return boost::copy_exception(T(str.c_str()));
		}

		static void	write(oarchive_type & oa, T const & ex)
		{
			oa << std::string(ex.what());
		}

	};




	// exception holder
	template<class OArchive>
	struct ExceptHolder
	{
		typedef boost::function<void(OArchive &)>	type;
	};

	namespace detail
	{
		template<class T, class ArchivePair, class Pro>
		class ExceptWrapper
		{
			T	ex_;

			typedef typename Pro::except_key_type	except_key_type;
			except_key_type	ex_key_;

		public:

			ExceptWrapper(T const & ex, except_key_type key)
				: ex_(ex)
				, ex_key_(key)
			{
			}

			void operator () (typename ArchivePair::oarchive_type & oa) const
			{
				oa << ex_key_;
				ExceptIOImpl<T, ArchivePair>::write(oa, ex_);
			}
		};
	}
	
	

	DEFINE_EXCEPTION_MSG(RpcUnknownException, std::runtime_error);


	template<class ArchivePair, class Pro>
	typename ExceptHolder<typename ArchivePair::oarchive_type>::type	current_except(uint32 ex_seed)
	{
		try
		{
			throw;		// rethrow the current exception
		}
		// catch the registered exceptions
#define	RPC_REG_EXCEP(except)			\
	catch(except const & ex) { return detail::ExceptWrapper<except, ArchivePair, Pro>(ex, \
		unique_hash::hash<typename Pro::except_key_type>(ex_seed, #except)); }	
#include <lv/rpc/RegisterExceptions.hpp>

		// catch unknown exceptions
		catch(...) 
		{
			return detail::ExceptWrapper<RpcUnknownException, ArchivePair, Pro>(RpcUnknownException(),\
				 unique_hash::hash<typename Pro::except_key_type>(ex_seed, "RpcUnknownException"));
		}

		return ExceptHolder<typename ArchivePair::oarchive_type>::type();
	}


	template<typename Key>
	uint32	exception_hash_seed()
	{
		std::list<std::string> except_list;

#define	RPC_REG_EXCEP(ex) except_list.push_back(#ex);
#include <lv/rpc/RegisterExceptions.hpp>

		return unique_hash::seed<Key>(except_list);
	}

	// no exception is found associated with the specified key.
	DEFINE_EXCEPTION_MSG(InvalidExceptionID, std::runtime_error);
	

	/**
	 * It's derived from boost::noncopyable to prevent unintended copying. Used by Client.
	 * @param Key an exception will be mapped to a value of integral type @a Key 
	 *	using UniqueHash. Usually an int16 type should be competent. Otherwise you 
	 *	can use an int32 type.
	 */
	template<class ArchivePair, class Pro>
	class Exceptions : boost::noncopyable
	{
		typedef typename Pro::except_key_type key_type;

		typedef boost::unordered_map<key_type, typename ExceptIOPtr<ArchivePair>::type>	except_map;

		except_map except_;

		uint32	seed_;

	public:

		Exceptions()
			: seed_(exception_hash_seed<key_type>())
		{

#define RPC_REG_EXCEP(ex) except_.insert(std::make_pair(unique_hash::hash<key_type>(seed_, #ex), \
		ExceptIOPtr<ArchivePair>::type(new ExceptIOImpl<ex, ArchivePair>())));
#include <lv/rpc/RegisterExceptions.hpp>

		}

		/**
		 * number of exception types registered.
		 */
		size_t	size() const
		{
			return except_.size();
		}

		uint32	seed() const
		{
			return seed_;
		}

		/**
		 * @exception InvalidExceptionID no exception found associated with @key.
		 * @exception boost::archive::archive_exception ? error serializing (loading) the exception.
		 */
		boost::exception_ptr get(key_type key, typename ArchivePair::iarchive_type & ia)
		{
			except_map::iterator it = except_.find(key);
			if(it == except_.end())
				throw InvalidExceptionID();
			else
				return it->second->get(ia);
		}
	};


} } 


#endif // LV_RPC_EXCEPTIONS_HPP