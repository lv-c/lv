// *********************************************************************
//  Color   version:  1.0   ¡¤  date: 11/15/2007
//  --------------------------------------------------------------------
//			ÑÕÉ«
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_COLOR_HPP
#define LV_COLOR_HPP

#include <boost/operators.hpp>
#include <boost/assert.hpp>
#include <lv/IntType.hpp>

namespace lv
{

	class Color : boost::addable<Color,
		boost::multipliable<Color,
		boost::equality_comparable<Color> > >
	{
	public:

		enum {
			ele_num = 4
		};

		typedef uint8	element;


		static Color const white;
		static Color const black;

		static Color const red;
		static Color const green;
		static Color const blue;

		static Color const yellow;		// (255, 255, 0)
		static Color const aqua;		// (0, 255, 255)
		static Color const fuchsia;		// (255, 0, 255)


		Color()
		{
			set(0, 0, 0, 0);
		}

		explicit Color(element color)
		{
			set(color, color, color, 255);
		}

		Color(element r, element g, element b, element a = 255)
		{
			set(r, g, b, a);
		}

		void	set(element r, element g, element b, element a = 255)
		{
			this->r(r);
			this->g(g);
			this->b(b);
			this->a(a);
		}

		void	get(element& r, element& g, element& b) const
		{
			r = this->r();
			g = this->g();
			b = this->b();
		}


		void	get(element& r, element& g, element& b, element& a) const
		{
			get(r, g, b);
			a = this->a();
		}

		uint32 argb() const
		{
			return (a() << 24) | (r() << 16) | (g() << 8) | b();
		}

		uint32 rgb() const
		{
			return (r() << 16) | (g() << 8) | b();
		}



		element	b()	const
		{
			return color_[0];
		}
		void b(element color)
		{
			this->color_[0] = color;
		}
		element g() const
		{
			return color_[1];
		}
		void g(element color)
		{
			this->color_[1] = color;
		}
		element r() const
		{
			return color_[2];
		}
		void r(element color)
		{
			this->color_[2] = color;
		}

		element a() const
		{
			return color_[3];
		}

		void a(element color)
		{
			this->color_[3] = color;
		}

	
		element	gray() const
		{
			static_cast<element>(r() * 0.3 + g() * 0.59 + b() * 0.11 + 0.5);
		}


		// operators
		element const & operator [] (size_t i) const
		{
			BOOST_ASSERT(i < ele_num);
			return color_[i];
		}

		element & operator [] (size_t i)
		{
			BOOST_ASSERT(i < ele_num);
			return color_[i];
		}


		Color & operator += (Color const & clr)
		{
			for(size_t i = 0; i < ele_num; ++i)
			{
				color_[i] = std::min(color_[i] + clr.color_[i], 255);
			}
			return *this;
		}

		Color & operator *= (Color const & clr)
		{
			for(size_t i = 0; i < ele_num; ++i)
			{
				color_[i] = color_[i] * clr.color_[i] / (255 * 255);
			}
			return *this;
		}

		friend bool operator == (Color const & lhs, Color const & rhs)
		{
			for(size_t i = 0; i < ele_num; ++i)
			{
				if(lhs.color_[i] != rhs.color_[i])
					return false;
			}
			return true;
		}

	private:

		element color_[ele_num];

	};
}

#endif // LV_COLOR_HPP