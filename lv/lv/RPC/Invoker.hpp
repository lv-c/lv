// *********************************************************************
//  Invoker   version:  1.0   ¡¤  date: 09/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RPC_INVOKER_HPP
#define LV_RPC_INVOKER_HPP

#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/invoke.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/at_key.hpp>

#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/transform.hpp>

#include <boost/static_assert.hpp>

#include <lv/Packet/Serializable.hpp>
#include <lv/Fusion.hpp>

namespace lv { namespace rpc { namespace detail {


	template<typename T>
	struct DefaultExtractor
	{
		typedef	T	result_type;

		/**
		 * @exception boost::archive::archive_exception ?
		 */
		template<class IArchive>
		inline void operator () (IArchive & ia, result_type & re) const
		{
			ia >> re;
		}
	};

	
	/// get the default extractor for type T (the user doesn't provide an extractor for it)
	template<typename T, class ParamExtractors, class Enable = void>
	struct Extractor
	{
		typedef DefaultExtractor<T>	type;

		static type	& get(ParamExtractors & )
		{
			static type ex;
			return ex;
		}
	};

	/// get the user provided extractor for type T 
	template<typename T, class ParamExtractors>
	struct Extractor<T, ParamExtractors, typename boost::enable_if<
		boost::fusion::result_of::has_key<ParamExtractors, T> >::type>
	{
		typedef typename boost::fusion::result_of::at_key<ParamExtractors, T>::type	type;

		static type & get(ParamExtractors & extractors)
		{
			return boost::fusion::at_key<T>(extractors);
		}
	};


	// T const * -> T *
	template<typename T>
	struct RemovePointerConst
	{
		typedef T type;
	};
	template<typename T>
	struct RemovePointerConst<T const *>
	{
		typedef T * type;
	};

	template<typename T, class ParamExtractors>
	struct RawParamType
	{
		typedef typename RemovePointerConst<
			typename boost::remove_const<
				typename boost::remove_reference<T>::type
			>::type
		>::type type;

		/// parameter type of the registered function cann't be a pointer type if you don't provide 
		/// an extractor for it.
		BOOST_STATIC_ASSERT((boost::fusion::result_of::has_key<ParamExtractors, type>::value ||
			! boost::is_pointer<type>::value));
	};

	template<class IArchive, class ParamExtractors> 
	class ExtractParam
	{
		IArchive & ia_;
		ParamExtractors	& ex_;
	public:
		ExtractParam(IArchive & ia, ParamExtractors & ex)
			: ia_(ia) 
			, ex_(ex)
		{
		}

		template<typename T>
		void operator () (T & t) const
		{
			Extractor<T, ParamExtractors>::get(ex_)(ia_, t);
		}
	};



	template<class ArchivePair>
	class InvokerBase
	{
	protected:

		typedef typename ArchivePair::iarchive_t iarchive_t;
		typedef typename ArchivePair::oarchive_t oarchive_t;
	public:

		typedef Savable<oarchive_t>					ResultHolder;
		typedef std::auto_ptr<ResultHolder>			ResultHolderPtr;

	};


	template<class Signature, class ArchivePair, class ParamExtractors>
	class Invoker : public InvokerBase<ArchivePair>
	{
		typedef boost::function<Signature>	function_type;
		function_type	f_;

		// result type of the function
		typedef typename function_type::result_type result_type;
		typedef boost::function_types::parameter_types<Signature> param_type;

		static unsigned const arity = function_type::arity;

		// The result type shouldn't be a pointer type
		BOOST_STATIC_ASSERT(! boost::is_pointer<result_type>::value);


		/// "Converts a type into a storable type by removing const qualifiers and references."
		template<typename T>
		struct Storable : boost::remove_const<typename boost::remove_reference<T>::type> {};

		// storable result type
		typedef typename Storable<result_type>::type storable_result_type;
		BOOST_STATIC_ASSERT(! boost::is_pointer<storable_result_type>::value);

	public:
		
		Invoker(boost::function<Signature> f)
			: f_(f)
		{
		}

		ResultHolderPtr operator ()(iarchive_t & ia, ParamExtractors & extractors)
		{
			return invoke(ia, extractors, boost::mpl::bool_<boost::is_same<result_type, void>::value>());
		}


	private:

		/// invokes the function and wraps the result.
		ResultHolderPtr	invoke(iarchive_t & ia, ParamExtractors & extractors, boost::mpl::false_ void_result)
		{
			return ResultHolderPtr(new SavableImpl<storable_result_type, oarchive_t>(invoke_impl(ia, extractors)));
		}

		ResultHolderPtr	invoke(iarchive_t & ia, ParamExtractors & extractors, boost::mpl::true_ void_result)
		{
			invoke_impl(ia, extractors);
			return ResultHolderPtr(new NullSavable<oarchive_t>());
		}


		result_type invoke_impl(iarchive_t & ia, ParamExtractors & extractors)
		{
			MplToFusionCons<typename boost::mpl::transform<param_type, RawParamType<boost::mpl::_, 
				ParamExtractors> >::type>::type params;
		
			boost::fusion::for_each(params, ExtractParam<iarchive_t, ParamExtractors>(ia, extractors));
			return boost::fusion::invoke(f_, params);
		}

	};


} } }

#endif // LV_RPC_INVOKER_HPP
