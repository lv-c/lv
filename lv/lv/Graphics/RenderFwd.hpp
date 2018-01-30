// *********************************************************************
//  RenderFwd   version:  1.0   ¡¤  date: 09/03/2008
//  --------------------------------------------------------------------
//		render system forward declaration
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_RENDERFWD_HPP
#define LV_RENDERFWD_HPP

#include <lv/IntType.hpp>

#include <memory>


namespace lv
{
	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;


	class Font;
	using FontPtr = std::shared_ptr<Font>;

	template<class>	class PointT;
	using Point = PointT<int32>;

	template<class>	class SizeT;
	using Size = SizeT<int32>;

	template<class>	class RectT;
	using Rect = RectT<int32>;

}


#endif