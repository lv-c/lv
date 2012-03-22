// *********************************************************************
//  Source   version:  1.0   ¡¤  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#if !BOOST_PP_IS_ITERATING

#ifndef LV_DATAFLOW_SOURCE_HPP
#define LV_DATAFLOW_SOURCE_HPP

#include <lv/IBufferManager.hpp>
#include <lv/Stream/OStreamFactory.hpp>

#include <lv/DataFlow/Fwd.hpp>
#include <lv/DataFlow/Config.hpp>
#include <lv/DataFlow/StreamProxy.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/function.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/boost_tuple.hpp>

namespace lv { namespace flow {

	/// thread-safe (as long as the BufferManager is thread-safe)
	template<class Key, class Port, class OArchive>
	class Source
	{
		typedef typename Key		key_type;
		typedef typename Port		port_type;
		typedef typename OArchive	oarchive_type;


		BufferManagerPtr	buf_manager_;

		boost::function<void(port_type const&, BufferPtr)>	callback_;

		port_type	port_;

		OStreamFactory	ostream_factory_;

		// It's a simple flag. So we don't have a mutex for it.
		volatile bool	enabled_;

	public:

		Source(port_type const & port, boost::function<void(port_type const &, BufferPtr)> const & callback, BufferManagerPtr buf_manager)
			: port_(port)
			, callback_(callback)
			, buf_manager_(buf_manager)
			, enabled_(true)
		{
		}

		void enable(bool enabled)
		{
			this->enabled_ = enabled;
		}

		bool enabled() const
		{
			return enabled_;
		}

		/**
		 * call the target function.
		 * @note Do NOT call this after the corresponding DataFlow object is destroyed.
		 */ 
#		define BOOST_PP_ITERATION_PARAMS_1 (3, (0, LV_DATAFLOW_MAX_ARITY, <lv/DataFlow/Source.hpp>))
#		include BOOST_PP_ITERATE()


		/**
		 * streaming. This is another way to call the target function. It may be faster than 
		 *	@a call at compilation time and the number of arguments is not limited.
		 * @note Do NOT call this after the corresponding DataFlow object is destroyed.
		 */
		detail::StreamProxy<oarchive_type, Source> stream(key_type const & key)
		{
			// returns an empty StreamProxy object if it's not enabled
			if(! enabled_)
			{
				return detail::StreamProxy<oarchive_type, Source>();
			}

			BufferPtr buf = buf_manager_->get();
			OStreamPtr raw_os = ostream_factory_.open(*buf);

			detail::StreamProxy<oarchive_type, Source> proxy(buf, raw_os, *this);
			proxy << key;

			return proxy;
		}

	private:

		class Serialize
		{
			oarchive_type & oa_;

		public:

			Serialize(oarchive_type & oa) : oa_(oa) {}

			template<typename T>
			void operator() (T const * t) const
			{
				// use RPC_REGISTER_CLASS to register your class
				// BOOST_STATIC_ASSERT((boost::serialization::implementation_level<T>::value != 
				// boost::serialization::object_class_info));
				oa_ << *t;
			}
		};

		template<class Tuple>
		void call_impl(key_type const & key, Tuple const & args)
		{
			BufferPtr buf = buf_manager_->get();

			OStreamPtr raw_os = ostream_factory_.open(*buf);
			oarchive_type oa(*raw_os);

			oa << key;
			boost::fusion::for_each(args, Serialize(oa));
			raw_os->flush();

			callback_(port_, buf);
		}

		template<class, class> friend class detail::StreamProxyImpl;

		void push(BufferPtr buf)
		{
			callback_(port_, buf);
		}

	};

} }

#endif // LV_DATAFLOW_SOURCE_HPP

#else

#define LV_DATAFLOW_call_params(z, n, data) T##n const & t##n
#define LV_DATAFLOW_pointer_types(z, n, data) T##n const *

#define n BOOST_PP_ITERATION()

#if n > 0
template<BOOST_PP_ENUM_PARAMS(n, typename T)>
#endif
void call(key_type const & key BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, LV_DATAFLOW_call_params, ~))
{
	// it's not enabled
	if(! enabled_)
	{
		return;
	}

	typedef boost::tuples::tuple<BOOST_PP_ENUM(n, LV_DATAFLOW_pointer_types, ~)> tuple_type;

#if n == 0
	tuple_type args;
#else
	tuple_type args(BOOST_PP_ENUM_PARAMS(n, &t));
#endif

	return this->call_impl(key, args);
}

#undef LV_DATAFLOW_call_params
#undef LV_DATAFLOW_pointer_types

#undef n

#endif