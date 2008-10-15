// *********************************************************************
//  ButtonBase   version:  1.0   ¡¤  date: 10/07/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BUTTONBASE_HPP
#define LV_BUTTONBASE_HPP

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

		virtual	void	on_mouse_enter(MouseEvent & e);
		virtual	void	on_mouse_leave(MouseEvent & e);

		virtual	void	on_button_down(MouseEvent & e);
		virtual	void	on_button_up(MouseEvent & e);

	};
}

#endif // LV_BUTTONBASE_HPP