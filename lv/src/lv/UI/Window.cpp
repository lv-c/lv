#include <lv/UI/Window.hpp>

namespace lv
{
	Window::Window()
		: visible_(true)
		, enabled_(true)
		, parent_(nullptr)
	{
	}

	
	void Window::render()
	{
		if (! visible())
		{
			return;
		}

		if (renderer_.get() != nullptr)
		{
			renderer_->render();
		}
	}

}