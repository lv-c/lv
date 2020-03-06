// *********************************************************************
//  Operations   version:  1.0   ¡¤  date: 2019/01/08
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2019 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/ContainerAdaptor/Tags.hpp>


namespace lv
{
	namespace container_adaptor
	{
		namespace detail
		{
			template<class T>
			void	clear_dispatch(T & t, container_tag)
			{
				t.clear();
			}


			//
			template<class T>
			void	resize_dispatch(T & t, size_t size, sequence_tag)
			{
				t.resize(size);
			}


			//
			template<class T, class V>
			void	push_dispatch(T & t, V && v, back_insertion_sequence_tag)
			{
				t.push_back(std::forward<V>(v));
			}

			template<class T, class V>
			void	push_dispatch(T & t, V && v, associate_tag)
			{
				t.insert(std::forward<V>(v));
			}


			//
			template<class T, class Iter>
			void	assign_dispatch(T & t, Iter begin, Iter end, container_tag)
			{
				t.assign(begin, end);
			}
		}


		template<class T>
		void	clear(T & t)
		{
			detail::clear_dispatch(t, container_category<T>());
		}


		template<class T>
		void	resize(T & t, size_t size)
		{
			detail::resize_dispatch(t, size, container_category<T>());
		}


		template<class T, class V>
		void	push(T & t, V && v)
		{
			detail::push_dispatch(t, std::forward<V>(v), container_category<T>());
		}


		template<class T, class Iter>
		void	assign(T & t, Iter begin, Iter end)
		{
			detail::assign_dispatch(t, begin, end, container_category<T>());
		}
	}


	using container_adaptor::clear;

	using container_adaptor::resize;

	using container_adaptor::push;

	using container_adaptor::assign;
}