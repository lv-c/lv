#include <lv/UI/ButtonBase.hpp>

namespace lv
{
	ButtonBase::ButtonBase()
		: mouse_over_(false)
		, pushed_(false)
	{
	}

	void ButtonBase::on_mouse_enter(MouseEvent & e)
	{
		mouse_over_ = true;
	}

	void ButtonBase::on_mouse_leave(MouseEvent & e)
	{
		mouse_over_ = false;
	}

	void ButtonBase::on_button_down(MouseEvent & e)
	{
		if (e.button() == Mouse::Left)
		{
			capture_input();
			pushed_ = true;

			e.consume();
		}
	}

	void ButtonBase::on_button_up(MouseEvent & e)
	{
		if (e.button() == Mouse::Left)
		{
			release_input();
			pushed_ = false;

			e.consume();
		}
	}
}