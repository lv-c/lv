// *********************************************************************
//  Manipulators   version:  1.0   ¡¤  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/BinaryOStream.hpp>
#include <lv/Exception.hpp>

#include <boost/serialization/split_member.hpp>

#include <type_traits>
#include <algorithm>
#include <limits>


namespace lv::bstream
{
	// check_equal

	DEFINE_EXCEPTION_MSG(CheckEqualError, std::runtime_error);

	
	namespace detail
	{
		template<class T, class Except>
		class check_equal_impl
		{
			T const &		t_;

			Except const &	ex_;

		public:
			check_equal_impl(T const & t, Except const & ex) 
				: t_(t) 
				, ex_(ex)
			{
			}

			/**
			 * @exception CheckEqualError
			 */
			friend BinaryIStream & operator >> (BinaryIStream & is, check_equal_impl && ce)
			{
				T val;
				is >> val;
				if (val != ce.t_)
				{
					throw ce.ex_;
				}

				return is;
			}
		};
	}

	template<class T>
	inline auto		check_equal(T const & t)
	{
		return detail::check_equal_impl<T, CheckEqualError>(t, CheckEqualError());
	}
	
	// you can specify your own exception type
	template<class T, class Except>
	inline auto		check_equal(T const & t, Except const & ex)
	{
		return detail::check_equal_impl<T, Except>(t, ex);
	}

	// forward

	class forward
	{
		std::streamoff off_;

	public:
		
		explicit forward(std::streamoff off) : off_(off) {}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, forward && fwd)
		{
			is.seekg(fwd.off_, std::ios_base::cur);
			return is;
		}

	};

	//
	class fill_n
	{
		std::size_t	size_;

		char	char_;

	public:
		
		fill_n(std::size_t size, char c) 
			: size_(size) 
			, char_(c)
		{
		}

		friend BinaryOStream & operator << (BinaryOStream & os, fill_n && fill)
		{
			std::size_t const buf_size = 128;
			char buf[buf_size];

			std::fill_n(buf, std::min(buf_size, fill.size_), fill.char_);

			std::size_t left = fill.size_;

			while (left > 0)
			{
				std::size_t sz = std::min(left, buf_size);

				os.write(buf, static_cast<std::streamsize>(sz));

				left -= sz;
			}

			return os;
		}
	};

	//

	class blank
	{
		std::size_t	size_;

	public:

		explicit blank(std::size_t size)
			: size_(size)
		{
		}

		template<class Archive>
		void	serialize(Archive & ar, unsigned int version)
		{
			boost::serialization::split_member(ar, *this, version);
		}

		void	load(BinaryIStream & is, unsigned int)
		{
			is >> forward(size_);
		}

		void	save(BinaryOStream & os, unsigned int) const
		{
			os << fill_n(size_, '\0');
		}
	};

	//

	class fixed_len_str
	{
		std::string	&	str_;

		std::size_t		size_;

	public:

		fixed_len_str(std::string & str, std::size_t size) 
			: str_(str)
			, size_(size) 
		{
		}

		template<class Archive>
		void	serialize(Archive & ar, unsigned int version)
		{
			boost::serialization::split_member(ar, *this, version);
		}

		void	load(BinaryIStream & is, unsigned int)
		{
			str_.resize(size_);

			if (size_ > 0)
			{
				is.read(&str_[0], static_cast<std::streamsize>(size_));

				for (size_t i = 0; i < size_; ++i)
				{
					if (str_[i] == '\0')
					{
						str_.resize(i);
						break;
					}
				}
			}
		}

		void	save(BinaryOStream & os, unsigned int) const
		{
			BOOST_ASSERT(str_.size() <= size_);

			std::size_t sz = std::min(str_.size(), size_);
			if (sz > 0)
			{
				os.write(&str_[0], static_cast<std::streamsize>(sz));
			}

			if (sz < size_)
			{
				os << fill_n(size_ - sz, '\0');
			}
		}
	};


	namespace detail
	{
		template<class SizeType, class Range>
		class variable_len_range_impl
		{
			Range &	range_;

		public:

			explicit variable_len_range_impl(Range & range)
				: range_(range)
			{
			}

			template<class Archive>
			void	serialize(Archive & ar, unsigned int version) const
			{
				boost::serialization::split_member(ar, *this, version);
			}

			void	load(BinaryIStream & is, unsigned int) const
			{
				SizeType size;
				is >> size;

				range_.resize(size);
				is >> range_;
			}

			void	save(BinaryOStream & os, unsigned int) const
			{
				BOOST_ASSERT(range_.size() <= std::numeric_limits<std::make_unsigned_t<SizeType> >::max());

				SizeType size = static_cast<SizeType>(range_.size());
				os << size << range_;
			}
		};
	}

	template<class SizeType, class Range>
	auto	variable_len_range(Range & range)
	{
		return detail::variable_len_range_impl<SizeType, Range>(range);
	}

	//

	namespace detail
	{
		template<class T>
		class pod_impl
		{
			T &		t_;

		public:

			explicit pod_impl(T & t)
				: t_(t)
			{
			}

			template<class Archive>
			void	serialize(Archive & ar, unsigned int version) const
			{
				boost::serialization::split_member(ar, *this, version);
			}

			void	load(BinaryIStream & is, unsigned int) const
			{
				is.read(reinterpret_cast<char *>(&t_), sizeof(t_));
			}

			void	save(BinaryOStream & os, unsigned int) const
			{
				os.write(reinterpret_cast<char const *>(&t_), sizeof(t_));
			}
		};
	}

	template<class T>
	std::enable_if_t<std::is_pod_v<T>, detail::pod_impl<T> >	pod(T & t)
	{
		return detail::pod_impl<T>(t);
	}

}

