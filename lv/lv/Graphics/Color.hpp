// *********************************************************************
//  Color   version:  1.0   ��  date: 11/15/2007
//  --------------------------------------------------------------------
//  ÿ����ɫ������һ���ֽڱ�ʾ����ģ�������������ɫ���ֽ���
//	N ���ֽ�({1, 3, 4})��֧�� r,g,b,a ��ǰ N ��
//	�� 4 ���ֽ�֧�� r,g,b,a�ĸ�������3 ���ֽ���֧�� alpha��
//	1 ���ֽ�ֻ֧�� r������������ʾ�Ҷ�ͼ
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2007 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************
#ifndef LV_COLOR_HPP
#define LV_COLOR_HPP

#include <functional>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
/*
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>
*/
namespace lv
{

	/** N: �����ֽ� */
	template<size_t N>
	class ColorT
	{
		BOOST_STATIC_ASSERT( N == 1 || N == 3 || N == 4);		
	public:
		typedef uint8 element;

		enum {
			bytes = N
		};

		static ColorT const white;
		static ColorT const black;
		static ColorT const red;
		static ColorT const green;
		static ColorT const blue;

		// ������ɫ������ʼ��Ϊ 0
		ColorT()
		{
			for(size_t i = 0; i < N; ++i)
				color_[i] = 0;
		}

		ColorT(element r, element g, element b)
		{
			set(r, g, b, element(255));
		}

		// Ϊʲô����ȱʡ�����ء�����Ϊ luabind ��֧��
		ColorT(element r, element g, element b, element a)
		{
			set(r, g, b, a);
		}


		template<size_t N2>
		ColorT(ColorT<N2> const& color)
		{
			if(N2 == N && N2 == 1)
				this->color_[0] = color.gray();
			else 
				set(color.r(), color.g(), color.b(), color.a());
		}


		// argb
		/*explicit ColorT(unsigned int color)
		{
			set(static_cast<element>(color >> 16), static_cast<element>(color >> 8),
				static_cast<element>(color), static_cast<element>(color >> 24));
		}*/

		// ��� N == 1��color_[0] = color, ���򽫡�r, g, b����Ϊ��color
		explicit ColorT(element color)
		{
			if(N == 1)
				this->color_[0] = color;
			else
				set(color, color, color);
		}
	
		size_t size()
		{
			return N;
		}

		// access the array directly
		element& operator [] (size_t i)
		{
			BOOST_ASSERT(i < N && "out of range");
			return color_[i];
		}

		element const& operator [] (size_t i) const
		{
			BOOST_ASSERT(i < N && "out of range");
			return color_[i];
		}

		unsigned int argb() const
		{
			return (a() << 24) | (r() << 16) | (g() << 8) | b();
		}

		/*
		void	argb(unsigned int color)
		{
			set(static_cast<element>(color >> 16), static_cast<element>(color >> 8),
				static_cast<element>(color), static_cast<element>(color >> 24));
		}*/

		unsigned int rgb() const
		{
			return (r() << 16) | (g() << 8) | b();
		}

		void	set(element r, element g, element b)
		{
			set(r, g, b, a());
		}

		void	set(element r, element g, element b, element a)
		{
			if(N == 1)
			{
				color_[0] = to_gray(r, g, b);
			}
			else
			{
				this->r(r);
				this->g(g);
				this->b(b);
				this->a(a);
			}
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
		
		static element to_gray(element r, element g, element b)
		{
			return static_cast<element>(r * 0.3 + g * 0.59 + b * 0.11 + 0.5);
		}
		element	b()	const
		{
			return color_[0];
		}
		// use gray() instead when N == 1
		void b(element color)
		{
			BOOST_ASSERT(N > 1 && "please use 'gray'");
			this->color_[0] = color;
		}
		element g() const
		{
			return (N > 1 ? color_[1] : color_[0]);
		}
		void g(element color)
		{
			BOOST_ASSERT(N > 1 && "please use 'gray'");
			this->color_[1] = color;
		}
		element r() const
		{
			return (N > 1 ? color_[2] : color_[0]);
		}
		void r(element color)
		{
			BOOST_ASSERT(N > 1 && "please use 'gray'");
			this->color_[2] = color;
		}
		// return (N == 4 ? color_[3] : 255);
		element a() const
		{
			return (N == 4 ? color_[3] : 255);
		}
		void a(element color)
		{
			if(N == 4)
				this->color_[3] = color;
		}
		// gray
		element gray() const
		{
			return (N == 1 ? color_[0] : to_gray(r(), g(), b()));
		}
		// will not change the alpha value
		void gray(element color)
		{
			if(N == 1)
				this->color_[0] = color;
			else
				set(color, color, color);
		}
	private:

		element	color_[N];
	};
	
	template<size_t N>
	ColorT<N> const ColorT<N>::white(255);
	
	template<size_t N>
	ColorT<N> const ColorT<N>::black(0);

	template<size_t N>
	ColorT<N> const ColorT<N>::red(255, 0, 0);

	template<size_t N>
	ColorT<N> const ColorT<N>::green(0, 255, 0);

	template<size_t N>
	ColorT<N> const ColorT<N>::blue(0, 0, 255);


	// ^o^. enabled if N > 1	(SFINAE)   ... ���ǵ�ΪʲôҪд�����
	/*template<size_t N>
	inline typename boost::enable_if<boost::mpl::greater<boost::mpl::size_t<N>	, boost::mpl::size_t<1> >, 
		bool>::type operator == (ColorT<N> const& lhs, ColorT<N> const& rhs)
	{
		return lhs.argb() == rhs.argb();
	}*/
	
	// ��������� LV_DISABLE_ALPHA, ���Ƚ� alpha ֵ
#ifdef LV_DISABLE_ALPHA

	inline size_t _clr_num(size_t N)
	{
		return std::min(N, static_cast<size_t>(3));
	}
#else
	inline size_t _clr_num(size_t N)
	{
		return N;
	}
#endif

	template<size_t N>
	inline bool operator == (ColorT<N> const& lhs, ColorT<N> const& rhs)
	{
		for(size_t i = 0; i < _clr_num(N); ++i)
		{
			if(lhs[i] != rhs[i])
				return false;
		}
		return true;
	}

	template<size_t N>
	inline bool operator != (ColorT<N> const& lhs, ColorT<N> const& rhs)
	{
		for(size_t i = 0; i < _clr_num(N); ++i)
		{
			if(lhs[i] != rhs[i])
				return true;
		}
		return false;
	}


	// N == 1 ʱ�Ŀ��ٱȽ�
	inline bool operator == (ColorT<1> const& lhs, ColorT<1> const& rhs)
	{
		return lhs[0] == rhs[0];
	}

	
	// ���������� (��������� LV_DISABLE_ALPHA, ���Ƚ� alpha ֵ)
	template<size_t N>
	struct ClrLess : public std::binary_function<ColorT<N>, ColorT<N>, bool>
	{
		bool operator () (ColorT<N> const& lhs, ColorT<N> const& rhs) const
		{
			for(size_t i = _clr_num(N) - 1; i < N ; --i)
			{
				if(lhs[i] < rhs[i])
					return true;
				else if(lhs[i] > rhs[i])
					return false;
			}
			return false;
		}
	};

}


#endif // LV_COLOR_HPP