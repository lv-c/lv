// *********************************************************************
//  NvpHelper   version:  1.0   ¡¤  date: 09/26/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZATION_NVPHELPER_HPP
#define LV_SERIALIZATION_NVPHELPER_HPP

#include <boost/serialization/nvp.hpp>

namespace lv
{
	template<class Archive>
	class NvpHelper
	{
		Archive &	ar_;

	public:

		NvpHelper *	_NVP_HELPER_A;
		NvpHelper *	_NVP_HELPER_B;
		
		explicit NvpHelper(Archive & ar)
			: ar_(ar)
		{
			_NVP_HELPER_A = this;
			_NVP_HELPER_B = this;
		}

		template<typename T>
		NvpHelper const & operator () (char const * name, T & t) const
		{
			ar_ & boost::serialization::make_nvp(name, t);
			return *this;
		}


		// used by library

		template<typename T>
		NvpHelper const & _serialize (char const * name, T & t) const
		{
			return (*this) (name, t);
		}

		template<typename T>
		NvpHelper const & _serialize (char const *, T & t, char const * name) const
		{
			return (*this) (name, t);
		}
	};

	template<class Archive>
	NvpHelper<Archive>	make_nvp_helper(Archive & ar)
	{
		return NvpHelper<Archive>(ar);
	}

	template<class Archive>
	NvpHelper<Archive const>	make_nvp_helper(Archive && ar)
	{
		return NvpHelper<Archive const>(ar);
	}
}

#define _NVP_HELPER_A(x, ...) _NVP_HELPER_OP(B, x, ##__VA_ARGS__)
#define _NVP_HELPER_B(x, ...) _NVP_HELPER_OP(A, x, ##__VA_ARGS__)
#define _NVP_HELPER_OP(next, x, ...)	_NVP_HELPER_A->_serialize(#x, ##__VA_ARGS__, x)._NVP_HELPER_##next

#define LV_NVP_HELPER(ar) lv::make_nvp_helper(ar)._NVP_HELPER_A

// how to use : LV_NVP_HELPER(ar) (a) (b) ("my_own_name", c);

#endif
