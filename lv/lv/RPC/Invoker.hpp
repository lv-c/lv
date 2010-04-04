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
#include <boost/mpl/assert.hpp>

#include <boost/static_assert.hpp>

#include <lv/Fusion.hpp>
#include <lv/Ref.hpp>

namespace lv { namespace rpc { namespace detail {

	namespace fusion = boost::fusion;
	namespace mpl = boost::mpl;

	template<typename T>
	struct DefaultExtractor
	{
		typedef	T	type;

		/**
		 * @exception boost::archive::archive_exception ?
		 */
		template<class IArchive>
		void operator () (IArchive & ia, type & re) const
		{
			// use RPC_REGISTER_CLASS to register your class
			//BOOST_STATIC_ASSERT((boost::serialization::implementation_level<type>::value != 
			//	boost::serialization::object_class_info));
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
		fusion::result_of::has_key<ParamExtractors, T> >::type>
	{
		typedef typename boost::remove_reference<
			typename fusion::result_of::at_key<ParamExtractors, T>::type	// it's a reference type
		>::type type;

		static type & get(ParamExtractors & extractors)
		{
			return fusion::at_key<T>(extractors);
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
		>::type	type;

		/// parameter type of the registered function can't be a pointer type if you don't provide 
		/// an extractor for it.
		BOOST_MPL_ASSERT_NOT((boost::is_pointer<type>));
	};

	template<class ParamExtractors, typename T>
	struct ExtractorParamType
	{
		typedef typename boost::remove_reference<
			typename fusion::result_of::at_key<ParamExtractors, T>::type	// reference type
		>::type::type type;
	};

	template<typename T, class ParamExtractors>
	class ParamType
	{
		typedef typename RawParamType<T, ParamExtractors>::type raw_param_type;
	public:
		typedef typename mpl::eval_if<
			fusion::result_of::has_key<ParamExtractors, raw_param_type>,
			ExtractorParamType<ParamExtractors, raw_param_type>,
			RawParamType<T, ParamExtractors>
		>::type type;
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

		template<typename OriType, typename T>
		void operator () (T & t) const
		{
			Extractor<typename RawParamType<OriType, ParamExtractors>::type, ParamExtractors>::get(ex_)(ia_, t);
		}
	};



	template<class ArchivePair>
	class InvokerBase
	{
	protected:

		typedef typename ArchivePair::iarchive_type iarchive_type;
		typedef typename ArchivePair::oarchive_type oarchive_type;

		template<typename T>
		class ResultWrapper
		{
			T	val_;
		public:

			ResultWrapper(T val) : val_(val) {}

			void operator() (oarchive_type & oa)
			{
				oa << val_;
			}
		};

	public:

		typedef boost::function<void(oarchive_type &)>		ResultHolder;

	};


	// custom for_each
	template <typename FirstU, typename First, typename Last, typename F>
	void for_each_linear(First const&, Last const&, F const&, mpl::true_)
	{
	}

	template <typename FirstU, typename First, typename Last, typename F>
	void for_each_linear(First const& first, Last const& last, F const& f, mpl::false_)
	{
		f.operator()<typename mpl::deref<FirstU>::type>(*first);
		for_each_linear<typename mpl::next<FirstU>::type>(fusion::next(first), last, f, 
			fusion::result_of::equal_to<typename fusion::result_of::next<First>::type, Last>());
	}


	template <typename SequenceU, typename Sequence, typename F>
	void for_each(Sequence& seq, F const& f)
	{
		for_each_linear<typename mpl::begin<SequenceU>::type>(fusion::begin(seq)
			, fusion::end(seq), f, fusion::result_of::equal_to<
			typename fusion::result_of::begin<Sequence>::type
			, typename fusion::result_of::end<Sequence>::type>());
	}


	template<class Signature, class ArchivePair, class ParamExtractors>
	class Invoker : public InvokerBase<ArchivePair>
	{
		typedef boost::function<Signature>	function_type;
		function_type	f_;

		// result type of the function
		typedef typename function_type::result_type result_type;
		typedef typename boost::function_types::parameter_types<Signature>::type param_type;

		static unsigned const arity = function_type::arity;

		// The result type shouldn't be a pointer type
		BOOST_MPL_ASSERT_NOT((boost::is_pointer<result_type>));


		/// "Converts a type into a storable type by removing const qualifiers and references."
		template<typename T>
		struct Storable : boost::remove_const<typename boost::remove_reference<T>::type> {};

		// storable result type
		typedef typename Storable<result_type>::type storable_result_type;
		BOOST_MPL_ASSERT_NOT((boost::is_pointer<storable_result_type>));

	public:
		
		Invoker(boost::function<Signature> f)
			: f_(f)
		{
		}

		ResultHolder operator ()(iarchive_type & ia, ParamExtractors & extractors)
		{
			return invoke(ia, extractors, mpl::bool_<boost::is_same<result_type, void>::value>());
		}


	private:

		/// invokes the function and wraps the result.
		ResultHolder	invoke(iarchive_type & ia, ParamExtractors & extractors, mpl::false_ void_result)
		{
			return ResultWrapper<result_type>(invoke_impl(ia, extractors));
		}

		ResultHolder	invoke(iarchive_type & ia, ParamExtractors & extractors, mpl::true_ void_result)
		{
			invoke_impl(ia, extractors);
			return ResultHolder();
		}


		result_type invoke_impl(iarchive_type & ia, ParamExtractors & extractors)
		{
			MplToFusionCons<typename mpl::transform<param_type, ParamType<mpl::_, 
				ParamExtractors> >::type>::type params;
		
			for_each<param_type>(params, ExtractParam<iarchive_type, ParamExtractors>(ia, extractors));
			return fusion::invoke(f_, params);
		}

	};


} } }

#endif // LV_RPC_INVOKER_HPP
