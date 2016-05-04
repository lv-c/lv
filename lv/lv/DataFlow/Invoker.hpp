// *********************************************************************
//  Invoker   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_INVOKER_HPP
#define LV_DATAFLOW_INVOKER_HPP

#include <lv/Ref.hpp>
#include <lv/Fusion.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/type_traits/is_pointer.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/invoke.hpp>

namespace lv { namespace flow { namespace detail {

	template<class T, class IArchive>
	struct ParamType
	{
		typedef typename boost::remove_const<
			typename boost::remove_reference<T>::type
		>::type type;

		// parameter types of the registered functions can't be pointer types
		BOOST_MPL_ASSERT_NOT((boost::is_pointer<type>));
	};

	template<class T>
	struct ParamType<T &, T>
	{
		// we shouldn't make a copy of the input archive.
		typedef RefWrapper<T>	type;
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
	struct Extractor<RefWrapper<IArchive>, IArchive>
	{
		void operator () (IArchive & ia, RefWrapper<IArchive> & t) const
		{
			t = boost::ref(ia);
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

	template<class Signature, class IArchive>
	class Invoker
	{

		typedef std::function<Signature>	function_type;
		function_type f_;
	
		typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;

	public:

		Invoker(function_type const & f)
			: f_(f)
		{
		}

		void operator() (IArchive & ia)
		{
			MplToFusionCons<typename boost::mpl::transform<parameter_types, ParamType<boost::mpl::_,
				IArchive> >::type>::type params;

			boost::fusion::for_each(params, ExtractParameters<IArchive>(ia));

			boost::fusion::invoke(f_, params);
		}
	};

} } }

#endif