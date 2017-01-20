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

#include <lv/Fusion.hpp>
#include <lv/Ref.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/invoke.hpp>

#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/transform.hpp>

#include <functional>
#include <type_traits>


namespace lv { namespace rpc { namespace detail {

	template<class T>
	struct ParamType
	{
		typedef std::remove_const_t<std::remove_reference_t<T> >	type;

		static_assert(! std::is_pointer<type>::value, "parameter types of the registered functions can't be pointer types");
	};

	template<class T, class IArchive>
	struct Extractor
	{
		void operator () (IArchive & ia, T & t) const
		{
			ia >> t;
		}
	};

	template<class IArchive>
	class ExtractParameters
	{
		IArchive & ia_;

	public:

		ExtractParameters(IArchive & ia)
			: ia_(ia)
		{
		}

		template<class T>
		void operator () (T & t) const
		{
			Extractor<T, IArchive>() (ia_, t);
		}
	};

	template<class ArchivePair>
	class InvokerBase
	{
	public:

		typedef std::function<void(typename ArchivePair::oarchive_type &)>		ResultHolder;

	};


	template<class Signature, class ArchivePair>
	class Invoker : public InvokerBase<ArchivePair>
	{
		typedef typename InvokerBase<ArchivePair>::ResultHolder	ResultHolder;

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


		typedef std::function<Signature>	function_type;
		function_type	fn_;

		// result type of the function
		typedef typename function_type::result_type result_type;
		typedef typename boost::function_types::parameter_types<Signature>::type param_type;

		static_assert(! std::is_pointer<result_type>::value, "The result type shouldn't be a pointer type");


	public:
		
		Invoker(std::function<Signature> fn)
			: fn_(fn)
		{
		}

		ResultHolder operator ()(iarchive_type & ia)
		{
			return invoke(ia, std::is_same<result_type, void>());
		}

	private:

		/// invokes the function and wraps the result.
		ResultHolder	invoke(iarchive_type & ia, std::false_type void_result)
		{
			return ResultWrapper<result_type>(invoke_impl(ia));
		}

		ResultHolder	invoke(iarchive_type & ia, std::true_type void_result)
		{
			invoke_impl(ia);
			return ResultHolder();
		}

		result_type invoke_impl(iarchive_type & ia)
		{
			typename MplToFusionCons<typename boost::mpl::transform<param_type, ParamType<boost::mpl::_> >::type>::type params;
		
			boost::fusion::for_each(params, ExtractParameters<iarchive_type>(ia));
			return boost::fusion::invoke(fn_, params);
		}

	};


} } }

#endif
