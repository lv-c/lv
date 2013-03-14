// *********************************************************************
//  Editbox   version:  1.0   ¡¤  date: 10/10/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_EDIT_HPP
#define LV_EDIT_HPP

#include <boost/function.hpp>

#include <lv/UI/Window.hpp>

namespace lv
{
	class Editbox : public Window
	{
	public:
		typedef boost::function<bool (wchar_t, Editbox const &)>	filter_type;

	protected:
		size_t	caret_index_;
		size_t	max_char_num_;

		bool	masked_;
		wchar_t	mask_char_;

		filter_type	filter_;

	public:

		Editbox();
		
		//	accessors
		bool	masked() const
		{
			return masked_;
		}
		void	set_masked(bool m) 
		{
			this->masked_ = m;
		}

		wchar_t	mask_char() const
		{
			return mask_char_;
		}
		void	set_mask_char(wchar_t c)
		{
			this->mask_char_ = c;
		}

		/**
		 * set the maximum number of characters that can be input.
		 * if @a num is larger than the size of the current text, the text will be resize
		 */
		void	set_max_char_num(size_t num);

		size_t	caret_index() const
		{
			return caret_index_;
		}
		void	set_caret_index(size_t index);

		/**
		 * set the filter. you can set an empty filter and NullFilter will be used instead.
		 */
		void	set_filter(filter_type filter);

	protected:

		virtual void	on_button_down(MouseEvent & e);
		virtual	void	on_key_down(KeyEvent & e);
		virtual	void	on_char(KeyEvent & e);

	};
}

#endif