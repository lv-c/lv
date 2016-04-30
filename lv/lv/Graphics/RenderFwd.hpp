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
	typedef std::shared_ptr<Texture>	TexturePtr;


	class Font;
	typedef std::shared_ptr<Font>	FontPtr;

	template<typename>	class PointT;
	typedef PointT<int32>	Point;

	template<typename>	class SizeT;
	typedef SizeT<int32>	Size;

	template<typename>	class RectT;
	typedef RectT<int32>	Rect;

}


#endif