// *********************************************************************
//  Invoker   version:  1.0   ¡¤  date: 10/25/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/Ref.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <type_traits>
#include <tuple>


namespace lv::flow::detail
{
	template<class T, class IArchive>
	struct ParamType
	{
		using type = std::remove_const_t<std::remove_reference_t<T> >;

		static_assert(!std::is_pointer_v<type>, "parameter types of the registered functions can't be pointer types");
	};

	template<class T>
	struct ParamType<T &, T>
	{
		// we shouldn't make a copy of the input archive.
		using type = RefWrapper<T>;
	};


	template<class Signature, class IArchive>
	struct ParametersType;

	template<class R, class ...T, class IArchive>
	struct ParametersType<R(T...), IArchive>
	{
		using type = std::tuple<typename ParamType<T, IArchive>::type...>;
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
			t = std::ref(ia);
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
		using function_type = std::function<Signature>;
		function_type	fn_;
	
	public:

		Invoker(function_type fn)
			: fn_(std::move(fn))
		{
		}

		void operator() (IArchive & ia)
		{
			typename ParametersType<Signature, IArchive>::type params;

			boost::fusion::for_each(params, ExtractParameters<IArchive>(ia));
			std::apply(fn_, std::move(params));
		}
	};

}
