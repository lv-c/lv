#include <lv/UI/Editbox.hpp>
#include <lv/UI/EditboxFilter.hpp>
#include <climits>

namespace lv
{

	Editbox::Editbox()
		: caret_index_(0)
		, max_char_num_(std::numeric_limits<size_t>::max())
		, masked_(false)
		, mask_char_('*')
		, filter_(NullFilter())
	{
	}

	void Editbox::on_button_down(MouseEvent & e)
	{
		if(e.button() == Mouse::Left)
		{
			capture_input();

			e.consume();
		}
	}

	void Editbox::on_key_down(KeyEvent & e)
	{
		switch(e.key())
		{
		case Key::ArrowLeft:
			if(caret_index_ > 0)
			{
				set_caret_index(caret_index_ - 1);
			}
			break;

		case Key::ArrowRight:
			set_caret_index(caret_index_ + 1);
			break;
		
		case Key::Backslash:
			if(caret_index_ > 0)
			{
				set_caret_index(caret_index_ - 1);
				text_.erase(caret_index_, 1);
			}
			break;
		case Key::Delete:
			text_.erase(caret_index_, 1);
			break;

		case Key::Home:
			set_caret_index(0);
			break;
		case Key::End:
			set_caret_index(text_.size());
			break;

		default:
			return;
		}

		e.consume();
	}

	void Editbox::on_char(KeyEvent & e)
	{
		if(filter_(e.character(), *this) && text_.size() < max_char_num_)
		{
			text_.push_back(e.character());
			set_caret_index(caret_index_ + 1);
		}

		e.consume();
	}

	void Editbox::set_caret_index(size_t index)
	{
		caret_index_ = std::min(index, text_.size());
	}

	void Editbox::set_max_char_num(size_t num)
	{
		max_char_num_ = num;

		if(text_.size() > num)
		{
			text_.resize(num);

			set_caret_index(caret_index_);
		}
	}

	void Editbox::set_filter(filter_type filter)
	{
		this->filter_ = filter ? filter : NullFilter();
	}

}