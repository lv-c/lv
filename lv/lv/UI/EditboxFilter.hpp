// *********************************************************************
//  EditboxFilter   version:  1.0   ¡¤  date: 10/10/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_EDITBOXFILTER_HPP
#define LV_EDITBOXFILTER_HPP

#include <list>

#include <lv/Foreach.hpp>
#include <lv/UI/Editbox.hpp>

namespace lv
{

	// TODO : add a regex filter ?  (or replace all these filters with a regex filter ?)

	struct NullFilter
	{
		bool operator () (wchar_t c, Editbox const &) const
		{
			return true;
		}
	};

	/// accepts only numbers
	struct NumberFilter
	{
		bool operator () (wchar_t c, Editbox const & edit) const
		{
			return std::isdigit(c, std::locale());
		}
	};

	struct BadwordFilter
	{
		BadwordFilter(std::wstring const & badwords)
			: badwords_(badwords)
		{
		}

		bool operator () (wchar_t c, Editbox const & edit) const
		{
			return (badwords_.find(c) == std::wstring::npos);
		}

	private:

		std::wstring	badwords_;
	};

	//////////////////////////////////////////////////////////////////////////
	// helpers

	struct FilterList
	{
		FilterList &	add_filter(Editbox::filter_type filter)
		{
			filters_.push_back(filter);
			return *this;
		}

		bool operator () (wchar_t c, Editbox const & edit)
		{
			foreach(Editbox::filter_type & f, filters_)
			{
				if(! f(c, edit))
					return false;
			}
			return true;
		}

	private:

		list<Editbox::filter_type>	filters_;
	};

	struct NotFilter
	{
		NotFilter(Editbox::filter_type filter) 
			: filter_(filter) 
		{
		}

		bool operator (wchar_t c, Editbox const & edit)
		{
			return ! filter(c, edit);
		}

	private:

		Editbox::filter_type	filter_;
	};

	

}

#endif // LV_EDITBOXFILTER_HPP