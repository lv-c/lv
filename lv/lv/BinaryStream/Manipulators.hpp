// *********************************************************************
//  Manipulators   version:  1.0   ��  date: 10/28/2008
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_BINARYSTREAM_MANIPULATORS_HPP
#define LV_BINARYSTREAM_MANIPULATORS_HPP

#include <lv/BinaryStream/BinaryIStream.hpp>
#include <lv/BinaryStream/BinaryOStream.hpp>
#include <lv/Exception.hpp>

#include <boost/serialization/split_member.hpp>

namespace lv { namespace bstream {


	// check_equal

	DEFINE_EXCEPTION_MSG(CheckEqualError, std::runtime_error);

	
	namespace detail
	{
		template<typename T, class Except>
		class check_equal_impl
		{
			T	t_;

			Except	ex_;

		public:
			check_equal_impl(T const & t, Except const & ex) 
				: t_(t) 
				, ex_(ex)
			{
			}

			/**
			 * @exception CheckEqualError
			 */
			friend BinaryIStream & operator >> (BinaryIStream & is, check_equal_impl const & ce)
			{
				T val;
				is >> val;
				if(val != ce.t_)
				{
					throw ce.ex_;
				}

				return is;
			}
		};
	}

	template<typename T>
	inline detail::check_equal_impl<T, CheckEqualError>	check_equal(T const & t)
	{
		return detail::check_equal_impl<T, CheckEqualError>(t, CheckEqualError());
	}
	
	// you can specify your own exception type
	template<typename T, class Except>
	inline detail::check_equal_impl<T, Except>	check_equal(T const & t, Except const & ex)
	{
		return detail::check_equal_impl<T, Except>(t, ex);
	}

	// forward

	class forward
	{
		std::streamoff off_;

	public:
		
		explicit forward(std::streamoff off) : off_(off) {}

		friend inline BinaryIStream & operator >> (BinaryIStream & is, forward const & fwd)
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

		friend BinaryOStream & operator << (BinaryOStream & os, fill_n const & fill)
		{
			std::size_t const buf_size = 128;
			char buf[buf_size];

			std::fill_n(buf, std::min(buf_size, fill.size_), fill.char_);

			std::size_t left = fill.size_;

			while(left > 0)
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

			if(size_ > 0)
			{
				is.read(&str_[0], static_cast<std::streamsize>(size_));

				for(size_t i = 0; i < size_; ++i)
				{
					if(str_[i] == '\0')
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
			if(sz > 0)
			{
				os.write(&str_[0], static_cast<std::streamsize>(sz));
			}

			if(sz < size_)
			{
				os << fill_n(size_ - sz, '\0');
			}
		}
	};


	namespace detail
	{
		template<typename SizeType, class Range>
		class variable_len_range_impl
		{
			Range &	range_;

		public:

			explicit variable_len_range_impl(Range & range)
				: range_(range)
			{
			}

			template<class Archive>
			void	serialize(Archive & ar, unsigned int version)
			{
				boost::serialization::split_member(ar, *this, version);
			}

			void	load(BinaryIStream & is, unsigned int)
			{
				SizeType size;
				is >> size;

				range_.resize(size);
				is >> range_;
			}

			void	save(BinaryOStream & os, unsigned int) const
			{
				SizeType size = static_cast<SizeType>(range_.size());
				os << size << range_;
			}
		};
	}

	template<typename SizeType, class Range>
	detail::variable_len_range_impl<SizeType, Range>	variable_len_range(Range & range)
	{
		return detail::variable_len_range_impl<SizeType, Range>(range);
	}

} }


#endif