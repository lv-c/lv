#include <lv/UI/Window.hpp>
#include <lv/UI/WindowRenderer.hpp>

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
		if(! visible())
			return;

		if(renderer_)
			renderer_->render();
	}

}