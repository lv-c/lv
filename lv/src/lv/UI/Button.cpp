#include <lv/UI/Button.hpp>

namespace lv
{

	ButtonBase()
	{
	}

	void Button::on_button_up(MouseEvent & e)
	{
		if(e.button() == Mouse::Left && pushed())
		{
			WindowEvent args(this);
			on_clicked(args);

			e.consume();
		}

		ButtonBase::on_button_up(e);
	}
}