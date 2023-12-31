// *********************************************************************
//  ButtonBase   version:  1.0   ��  date: 10/07/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/UI/Window.hpp>

namespace lv
{
	class ButtonBase : public Window
	{
	protected:

		bool	mouse_over_;

		bool	pushed_;

	public:

		ButtonBase();

		bool	mouse_over() const
		{
			return mouse_over_;
		}

		bool	pushed() const 
		{
			return pushed_;
		}

	protected:

		void	on_mouse_enter(MouseEvent & e) override;
		void	on_mouse_leave(MouseEvent & e) override;

		void	on_button_down(MouseEvent & e) override;
		void	on_button_up(MouseEvent & e) override;

	};
}