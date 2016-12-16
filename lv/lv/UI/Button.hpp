// *********************************************************************
//  Button   version:  1.0   ¡¤  date: 10/09/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BUTTON_HPP
#define LV_BUTTON_HPP

#include <lv/UI/ButtonBase.hpp>

namespace lv
{
	class Button : public ButtonBase
	{
	protected:

		void	on_button_up(MouseEvent & e) override;

		virtual	void	on_clicked(WindowEvent & e) {}
	};
}

#endif