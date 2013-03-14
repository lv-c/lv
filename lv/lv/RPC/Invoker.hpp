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

#include <boost/function.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/invoke.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/assert.hpp>

namespace lv { namespace rpc { namespace detail {

	template<class T>
	struct ParamType
	{
		typedef typename boost::remove_const<
			typename boost::remove_reference<T>::type
		>::type type;

		// parameter types of the registered functions can't be pointer types
		BOOST_MPL_ASSERT_NOT((boost::is_pointer<type>));
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


	template<class Signature, class ArchivePair>
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

	public:
		
		Invoker(boost::function<Signature> f)
			: f_(f)
		{
		}

		ResultHolder operator ()(iarchive_type & ia)
		{
			return invoke(ia, boost::mpl::bool_<boost::is_same<result_type, void>::value>());
		}

	private:

		/// invokes the function and wraps the result.
		ResultHolder	invoke(iarchive_type & ia, boost::mpl::false_ void_result)
		{
			return ResultWrapper<result_type>(invoke_impl(ia));
		}

		ResultHolder	invoke(iarchive_type & ia, boost::mpl::true_ void_result)
		{
			invoke_impl(ia);
			return ResultHolder();
		}

		result_type invoke_impl(iarchive_type & ia)
		{
			MplToFusionCons<typename boost::mpl::transform<param_type, ParamType<boost::mpl::_> >::type>::type params;
		
			boost::fusion::for_each(params, ExtractParameters<IArchive>(ia));
			return boost::fusion::invoke(f_, params);
		}

	};


} } }

#endif
