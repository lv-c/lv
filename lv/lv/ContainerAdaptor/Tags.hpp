// *********************************************************************
//  Tags   version:  1.0   ¡¤  date: 2019/01/07
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <type_traits>


namespace lv::container_adaptor
{
	struct non_container_tag {};

	struct container_tag {};

	struct sequence_tag : virtual public container_tag {};
	struct associate_tag : virtual public container_tag {};

	struct contiguous_sequence_tag : virtual public sequence_tag {};
	struct back_insertion_sequence_tag : virtual public sequence_tag {};


	//

	template<class T, class = void>
	struct container_category : non_container_tag {};


	//

	template<class T>
	constexpr bool	is_container_v = std::is_base_of_v<container_tag, container_category<T> >;
}