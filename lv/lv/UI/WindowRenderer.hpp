// *********************************************************************
//  WindowRenderer   version:  1.0   ¡¤  date: 10/10/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_WINDOWRENDERER_HPP
#define LV_WINDOWRENDERER_HPP


namespace lv
{
	// use type-erasion to avoid dynamic_cast (or static_cast)

	class IWindowRenderer
	{
	public:

		virtual	void	render() = 0;
	};


	template<class T>
	class WindowRendererT : public IWindowRenderer
	{
	protected:

		T	&	window_;

	public:

		WindowRendererT(T & window)
			: window_(window)
		{
		}

		virtual	void	render() = 0;
	};
}

#endif