// *********************************************************************
//  MovableOnly   version:  1.0   ¡¤  date: 2018/07/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2018 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


namespace lv
{
	class MovableOnly
	{
	protected:

		constexpr MovableOnly() = default;
		~MovableOnly() = default;

		MovableOnly(MovableOnly &&) = default;
		MovableOnly & operator = (MovableOnly &&) = default;

		MovableOnly(MovableOnly const &) = delete;
		MovableOnly & operator = (MovableOnly const &) = delete;

	};
}