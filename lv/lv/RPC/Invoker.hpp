// *********************************************************************
//  Invoker   version:  1.0   ¡¤  date: 09/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING


#ifndef LV_RPC_INVOKER_HPP
#define LV_RPC_INVOKER_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/enum_shifted_params.hpp>

#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/fusion/sequence/intrinsic/has_key.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>

#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/static_assert.hpp>

#include <lv/Packet/Serializable.hpp>

namespace lv { namespace rpc { namespace detail {


	template<typename T>
	struct DefaultExtractor
	{
		typedef	T	result_type;

		/**
		 * @exception boost::archive::archive_exception ?
		 */
		template<class ArchivePair>
		inline result_type	operator () (typename ArchivePair::iarchive_t & ia)
		{
			result_type	param;
			ia >> param;

			return param;
		}
	};


	/**
	 * get the extractor for type T
	 */
	template<typename T, class ParamExtractors, class Enable = void>
	struct Extractor
	{
		typedef DefaultExtractor<T>	type;

		static type	get(ParamExtractors & )
		{
			return type();
		}
	};

	template<typename T, class ParamExtractors>
	struct Extractor<T, ParamExtractors, typename boost::enable_if<
		boost::fusion::result_of::has_key<ParamExtractors, T> >::type>
	{
		typedef typename boost::fusion::result_of::at_key<ParamExtractors, T>::type	type;

		static type get(ParamExtractors & extractors)
		{
			return boost::fusion::at_key<T>(extractors);
		}
	};




	template<class ArchivePair, class ParamExtractors>
	class Invoker
	{
	protected:

		struct invoker_tag{};

		typedef typename ArchivePair::iarchive_t iarchive_t;
		typedef typename ArchivePair::oarchive_t oarchive_t;
	public:

		typedef Savable<iarchive_t, invoker_tag>	ResultHolder;

		virtual	std::auto_ptr<ResultHolder> invoke(iarchive_t & ia, ParamExtractors & extractors) = 0;

		virtual	std::auto_ptr<Invoker> clone() = 0;
	};


	template<class Signature, class ArchivePair, class ParamExtractors>
	class InvokerImpl : public Invoker<ArchivePair, ParamExtractors>
	{
		boost::function<Signature>	f_;

		// result type of the function
		typedef typename boost::function_traits<Signature>::result_type result_type;

		BOOST_STATIC_ASSERT(! boost::is_pointer<result_type>::value && "The result type shouldn't be a pointer type");


		/// "Converts a type into a storable type by removing const qualifiers and references."
		template<typename T>
		struct Storable : boost::remove_const<typename boost::remove_reference<T>::type> {};

		//
		typedef typename Storable<result_type>::type storable_result_type;
		BOOST_STATIC_ASSERT(! boost::is_pointer<storable_result_type>::value);

	public:
		
		InvokerImpl(boost::function<Signature> f)
			: f_(f)
		{
		}

		virtual	std::auto_ptr<ResultHolder> invoke(iarchive_t & ia, ParamExtractors & extractors)
		{
			return invoke_proxy<result_type>(ia, extractors);
		}

		virtual	std::auto_ptr<Invoker> clone()
		{
			return std::auto_ptr<Invoker>(new InvokerImpl(*this));
		}


	private:

		/// invokes the function and wrap the result.
		template<typename ResultType>
		std::auto_ptr<ResultHolder>	invoke_proxy(iarchive_t & ia, ParamExtractors & extractors)
		{
			return std::auto_ptr<ResultHolder>(new SavableImpl<storable_result_type, oarchive_t, invoker_tag>(invoke_impl(ia, extractors)));
		}

		template<>
		std::auto_ptr<ResultHolder>	invoke_proxy<void>(iarchive_t & ia, ParamExtractors & extractors)
		{
			invoke_impl(ia, extractors);
			return std::auto_ptr<ResultHolder>(new NullSavable());
		}


		result_type invoke_impl(iarchive_t & ia, ParamExtractors & extractors)
		{
			// extracts all the parameters
#			define BOOST_PP_ITERATION_PARAMS_1(3, (1, boost::function_traits<Signature>::arity, <lv/RPC/Invoker.hpp>))
#			include BOOST_PP_ITERATE()

			return f_(BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(boost::function_traits<Signature>::arity), arg));
		}

	};


} } }

#endif // LV_RPC_INVOKER_HPP

#else

#define n BOOST_PP_ITERATION()

#define	param_type arg##n##_type

// typedef the parameter type
BOOST_PP_CAT(typedef typename boost::function_traits<Signature>::, param_type)	param_type;
// extract the parameter
Extractor<param_type, ParamExtractors>::type BOOST_PP_CAT(arg, n) = Extractor<param_type, ParamExtractors>::get(extractors);


#undef n
#undef param_type

#endif 
