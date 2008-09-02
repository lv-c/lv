// *********************************************************************
//  ImageSet   version:  1.0   ¡¤  date: 08/23/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_IMAGESET_HPP
#define LV_IMAGESET_HPP

#include <vector>
#include <lv/Graphics/Rect.hpp>

namespace lv
{
	class ImagePos
	{
	public:
		Rect	bbox;		// Bound box
		Point	offset;

		ImagePos(Rect const & bbox, Point const & offset)
			: bbox(bbox)
			, offset(offset)
		{
		}
	};


	class ImageSet : public std::vector<ImagePos>
	{

	};
}


#endif // LV_IMAGESET_HPP