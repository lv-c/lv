// *********************************************************************
//  RenderFwd   version:  1.0   ¡¤  date: 09/03/2008
//  --------------------------------------------------------------------
//		render system forward declaration
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <memory>


namespace lv
{
	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;


	class Font;
	using FontPtr = std::shared_ptr<Font>;

	template<class>	class PointT;
	using Point = PointT<int32_t>;

	template<class>	class SizeT;
	using Size = SizeT<int32_t>;

	template<class>	class RectT;
	using Rect = RectT<int32_t>;

}
