// *********************************************************************
//  ColorRect   version:  1.0   ¡¤  date: 08/18/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/operators.hpp>
#include <boost/assert.hpp>
#include <lv/Graphics/Color.hpp>

namespace lv
{
	class ColorRect : boost::addable<ColorRect,
		boost::multipliable<ColorRect,
		boost::equality_comparable<ColorRect> > >
	{
	public:
	
		enum {
			ele_num = 4
		};

		Color	top_left;
		Color	top_right;
		Color	bottom_left;
		Color	bottom_right;


		static	ColorRect	white;


		/**
		 * Default constructor.
		 */
		ColorRect()
		{
		}

		/**
		 * Constructs a ColorRect object from a Color object. 
		 * All the four colors are set to @a color.
		 */
		explicit ColorRect(Color const & color)
			: top_left(color)
			, top_right(color)
			, bottom_left(color)
			, bottom_right(color)
		{
		}

		ColorRect(Color const & top_left, Color const & top_right, Color const & bottom_left, Color const & bottom_right)
			: top_left(top_left)
			, top_right(top_right)
			, bottom_left(bottom_left)
			, bottom_right(bottom_right)
		{
		}

		
		/**
		 * Set all the four colors to @a color.
		 */
		void	set(Color const & color)
		{
			set(color, color, color, color);
		}

		void	set(Color const & top_left, Color const & top_right, Color const & bottom_left, Color const & bottom_right)
		{
			this->top_left = top_left;
			this->top_right = top_right;
			this->bottom_left = bottom_left;
			this->bottom_right = bottom_right;
		}


		// operators
		Color const & operator[] (size_t i) const
		{
			BOOST_ASSERT(i < ele_num);
			return this->*mem_array[i];
		}

		Color & operator[] (size_t i)
		{
			BOOST_ASSERT(i < ele_num);
			return this->*mem_array[i];
		}

		ColorRect & operator += (ColorRect const & cr)
		{
			for (size_t i = 0; i < ele_num; ++i)
			{
				(*this)[i] += cr[i];
			}
		}

		ColorRect & operator *= (ColorRect const & cr)
		{
			for (size_t i = 0; i < ele_num; ++i)
			{
				(*this)[i] *= cr[i];
			}
		}

		friend bool operator == (ColorRect const & lhs, ColorRect const & rhs)
		{
			for (size_t i = 0; i < ele_num; ++i)
			{
				if (lhs[i] != rhs[i])
				{
					return false;
				}
			}

			return true;
		}

	private:
		
		static Color ColorRect::* const mem_array[ele_num]; 
	};
}
