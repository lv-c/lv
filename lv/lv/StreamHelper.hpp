// *********************************************************************
//  StreamHelper   version:  1.0   ��  date: 2013/08/23
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2013 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/IntType.hpp>

#include <boost/io/ios_state.hpp>

#include <ostream>
#include <iomanip>


namespace lv
{

	// TODO : supports wostream (template)

	namespace detail
	{
		template<class T>
		class hex_impl
		{
			T	t_;

		public:

			hex_impl(T t) : t_(t) {}

			template<class C, class Tr>
			friend std::basic_ostream<C, Tr> & operator << (std::basic_ostream<C, Tr> & os, hex_impl const & h)
			{
				boost::io::ios_flags_saver ias(os);

				if ((os.flags() & std::ios_base::showbase) && os.fill() == '0')
				{
					// std::showbase do not cooperate good with std::setw 

					int width = static_cast<int>(os.width());

					os << std::setw(0) << "0x" << std::setw(std::max(0, width - 2)) << std::noshowbase;
				}

				return os << std::hex << lv::widen_int(h.t_);
			}
		};
	}


	template<class T>
	detail::hex_impl<T> hex(T t)
	{
		return detail::hex_impl<T>(t);
	}


	//

	class write_tabs
	{
		size_t	count_;

		bool	expand_;

	public:

		explicit write_tabs(size_t count, bool expand = false)
			: count_(count)
			, expand_(expand)
		{
		}

		template<class C, class Tr>
		friend std::basic_ostream<C, Tr> & operator << (std::basic_ostream<C, Tr> & os, write_tabs tabs)
		{
			if (tabs.expand_)
			{
				for (size_t i = 0; i < tabs.count_ * 4; ++i)
				{
					os << ' ';
				}
			}
			else
			{
				for (size_t i = 0; i < tabs.count_; ++i)
				{
					os << '\t';
				}
			}

			return os;
		}
	};

	//

	namespace detail
	{
		template<class T, class C, class Tr>
		std::enable_if_t<std::is_arithmetic_v<T> >	write(std::basic_ostream<C, Tr> & os, T const & t, bool use_hex, int setw)
		{
			if (setw != -1)
			{
				os << std::setw(setw);
			}

			if (use_hex)
			{
				os << hex(t);
			}
			else
			{
				os << t;
			}
		}

		template<class T, class C, class Tr>
		std::enable_if_t<!std::is_arithmetic_v<T> >	write(std::basic_ostream<C, Tr> & os, T const & t, bool use_hex, int setw)
		{
			os << t;
		}


		template<class Range>
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

			template<class C, class Tr>
			friend std::basic_ostream<C, Tr> & operator << (std::basic_ostream<C, Tr> & os, write_range_impl const & r)
			{
				bool first = true;

				for (auto const & v : r.range_)
				{
					if (!first)
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

	template<class Range>
	detail::write_range_impl<Range>	write_range(Range const & range, char const * delimiter = ", ", bool use_hex = false,
		int setw = -1)
	{
		return detail::write_range_impl<Range>(range, delimiter, use_hex, setw);
	}


	//

	template<size_t Precision>
	class fixed
	{
		double	val_;

	public:

		explicit fixed(double val) : val_(val) {}

		template<class C, class Tr>
		friend std::basic_ostream<C, Tr> & operator << (std::basic_ostream<C, Tr> & os, fixed const & f)
		{
			boost::io::ios_flags_saver ias(os);
			return os << std::fixed << std::setprecision(Precision) << f.val_;
		}
	};
}
