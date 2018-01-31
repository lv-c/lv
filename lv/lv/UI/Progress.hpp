// *********************************************************************
//  Progress   version:  1.0   ¡¤  date: 10/09/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/assert.hpp>

#include <lv/UI/Window.hpp>

namespace lv
{

	class Progress : public Window
	{
	protected:

		size_t	step_;

		size_t	progress_;

		size_t	max_progress_;


	public:

		Progress()
			: step_(1)
			, progress_(0)
			, max_progress_(100)
		{
		}

		size_t	step() const
		{
			return step_;
		}

		void	set_step(size_t s)
		{
			this->step_ = s;
		}

		size_t	progress() const
		{
			return progress_;
		}

		void	set_progerss(size_t pro)
		{
			BOOST_ASSERT(pro <= max_progress_);

			this->progress_ = pro;
			bound();
		}

		size_t	max_progress() const
		{
			return max_progress_;
		}

		void	set_max_progress(size_t max_pro)
		{
			this->max_progress_ = max_pro;
			bound();
		}

		/// step
		void	forward()
		{
			progress_ += step_;
			bound();
		}

	protected:

		void	bound()
		{
			progress_ = std::min(progress_, max_progress_);
		}

	};
}