// *********************************************************************
//  Window   version:  1.0   ¡¤  date: 10/07/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_WINDOW_HPP
#define LV_WINDOW_HPP

#include <memory>
#include <string>
using std::wstring;

#include <lv/Graphics/Rect.hpp>
#include <lv/UI/EventArgs.hpp>
#include <lv/UI/WindowRenderer.hpp>

namespace lv
{
	class Window
	{
	protected:

		bool	visible_;

		bool	enabled_;

		Rect	rect_;

		wstring	text_;

		Window	* parent_;

		std::unique_ptr<IWindowRenderer>	renderer_;

	public:

		Window();

		// accessors

		void	set_renderer(std::unique_ptr<IWindowRenderer> renderer)
		{
			this->renderer_ = std::move(renderer);
		}

		Window *	parent() const
		{
			return parent_;
		}
	
		bool	visible() const
		{
			return visible_;
		}

		void	set_visible(bool visible)
		{
			this->visible_ = visible;
			request_redraw();
		}

		bool	enabled() const
		{
			return enabled_;
		}

		void	set_enabled(bool enabled)
		{
			this->enabled_ = enabled;
			request_redraw();
		}

		Rect const &	rect() const
		{
			return rect_;
		}

		wstring const &	text() const
		{
			return text_;
		}

		void	set_text(wstring const & text)
		{
			this->text_ = text;
		}

		/**
		 * render this window if it's visible. 
		 * you can provide a renderer or override this function to perform the render
		 */
		virtual void	render();

	protected:

	
		void	set_parent(Window * parent)
		{
			this->parent_ = parent;
		}

		// input events

		virtual void	on_mouse_move(MouseEvent & e) {}
		virtual	void	on_mouse_enter(MouseEvent & e) {}
		virtual	void	on_mouse_leave(MouseEvent & e) {}
		virtual	void	on_button_down(MouseEvent & e) {}
		virtual	void	on_button_up(MouseEvent & e) {}
		virtual	void	on_mouse_click(MouseEvent & e) {}

		virtual	void	on_key_down(KeyEvent & e) {}
		virtual	void	on_key_up(KeyEvent & e) {}
		virtual	void	on_char(KeyEvent & e) {}


		void	request_redraw();

		void	capture_input();
		void	release_input();
	};
}


#endif