#include <lv/UI/Window.hpp>

namespace lv
{
	Window::Window()
		: visible_(true)
		, enabled_(true)
		, parent_(NULL)
	{
	}

	
	void Window::render()
	{
		if (! visible())
		{
			return;
		}

		if (renderer_.get() != NULL)
		{
			renderer_->render();
		}
	}

}