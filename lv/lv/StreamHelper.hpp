// *********************************************************************
//  StreamHelper   version:  1.0   ¡¤  date: 2013/08/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_STREAMHELPER_HPP
#define LV_STREAMHELPER_HPP

#include <lv/IntType.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/io/ios_state.hpp>

#include <ostream>

namespace lv
{

	// TODO : supports wostream (template)

	namespace detail
	{
		template<typename T>
		class hex_impl
		{
			T	t_;

		public:

			hex_impl(T t) : t_(t) {}

			friend std::ostream & operator << (std::ostream & os, hex_impl const & h)
			{
				boost::io::ios_flags_saver ias(os);

				if((os.flags() & std::ios_base::showbase) && os.fill() == '0')
				{
					// std::showbase do not cooperate good with std::setw 

					int width = os.width();

					os << std::setw(0) << "0x" << std::setw(std::max(0, width - 2)) << std::noshowbase;
				}

				return os << std::hex << lv::widen_int(h.t_);
			}
		};
	}


	template<typename T>
	detail::hex_impl<T> hex(T t)
	{
		return detail::hex_impl<T>(t);
	}


	//

	class write_tabs
	{
		size_t	count_;

	public:

		explicit write_tabs(size_t count)
			: count_(count)
		{
		}

		friend std::ostream & operator << (std::ostream & os, write_tabs tabs)
		{
			for(size_t i = 0; i < tabs.count_; ++i)
			{
				os << '\t';
			}

			return os;
		}
	};

	//

	namespace detail
	{
		template<typename T>
		typename boost::enable_if<boost::is_arithmetic<T> >::type	write(std::ostream & os, T const & t, bool use_hex, int setw)
		{
			if(setw != -1)
			{
				os << std::setw(setw);
			}

			if(use_hex)
			{
				os << hex(t);
			}
			else
			{
				os << t;
			}
		}

		template<typename T>
		typename boost::disable_if<boost::is_arithmetic<T> >::type	write(std::ostream & os, T const & t, bool use_hex, int setw)
		{
			os << t;
		}


		template<typename Range>
		class write_range_impl
		{
			Range const &	range_;

			char const *	delimiter_;

			bool	use_hex_;

			int		setw_;

		public:

			write_range_impl(Range const & range, char const * delimiter, bool use_hex, int setw)
				: range_(range)
				, delimiter_(delimiter)
				, use_hex_(use_hex)
				, setw_(setw)
			{
			}

			friend std::ostream & operator << (std::ostream & os, write_range_impl const & r)
			{
				typedef boost::range_value<Range>::type	type;

				bool first = true;

				foreach(type const & v, r.range_)
				{
					if(! first)
					{
						os << r.delimiter_;
					}

					first = false;

					write(os, v, r.use_hex_, r.setw_);
				}

				return os;
			}
		};
	}

	template<typename Range>
	detail::write_range_impl<Range>	write_range(Range const & range, char const * delimiter = ", ", bool use_hex = false,
		int setw = -1)
	{
		return detail::write_range_impl<Range>(range, delimiter, use_hex, setw);
	}
}

#endif