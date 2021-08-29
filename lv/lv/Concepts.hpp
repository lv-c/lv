// *********************************************************************
//  Concepts   version:  1.0   ¡¤  date: 08/12/2021
//  --------------------------------------------------------------------
//
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2021 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <type_traits>


namespace lv
{
	template<class T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

	template<class T>
	concept Pod = std::is_trivial_v<T> && std::is_standard_layout_v<T>;
}