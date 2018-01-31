// *********************************************************************
//  PlainISerializer   version:  1.0   ¡¤  date: 2014/03/31
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2014 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/lvlib2.hpp>
#include <lv/LuaArchive/Tags.hpp>
#include <lv/LuaArchive/Common.hpp>
#include <lv/ContainerAdaptor/Adaptor.hpp>
#include <lv/Buffer.hpp>
#include <lv/Ensure.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>


namespace lv::lua::archive
{
	struct Token
	{
		enum Type
		{
			None,
			Symbol,
			Number,
			String,
			Literal
		};

		using iterator = ConstBufferRef::iterator;

		Token(iterator begin, iterator end, Type type)
			: begin(begin)
			, end(end)
			, type(type)
			, is_key(false)
		{
		}

		iterator	begin;

		iterator	end;

		Type	type;

		bool	is_key;

		size_t	size() const
		{
			return end - begin;
		}

		std::string	str() const
		{
			return std::string(begin, end);
		}

		bool operator == (char const * s) const
		{
			size_t len = strlen(s);
			return (len == size() && std::equal(begin, end, s));
		}

		static char const *	name(Token::Type type);
	};

	class UnmatchedTokenType : public std::runtime_error
	{
	public:
		UnmatchedTokenType(Token const & token, Token::Type expected)
			: std::runtime_error(std::string("unmatched token type:") + Token::name(expected) 
			+ " expected, " + Token::name(token.type) + " given:" + token.str())
		{
		}
	};


	class Parser
	{
		ConstBufferRef	holder_;

		using iterator = ConstBufferRef::iterator;

		iterator	cur_;

		iterator	end_;

	public:

		using is_loading = boost::mpl::true_;
		using is_saving = boost::mpl::false_;

		Parser(ConstBufferRef buf);

		Token	next_token();

		template<class T>
		Parser & operator >> (T & t)
		{
			load(*this, t);
			return *this;
		}

		template<class T>
		Parser & operator & (T & t)
		{
			return *this >> t;
		}

		bool	read_if(char c);

		bool	read_if(char const * lit);

		void	rollback(Token const & token);

	private:

		bool	test_and_forward(iterator it, char test);

		template<class T>
		void	next(T)
		{
			while (cur_ != end_ && T::eval(*cur_))
			{
				cur_++;
			}
		}
	};


	namespace detail
	{
		inline void	expect(Token const & token, Token::Type type)
		{
			LV_ENSURE(token.type == type, UnmatchedTokenType(token, type));
		}

		struct is_space
		{
			static bool	eval(char c);
		};

		struct is_number
		{
			static bool	eval(char c);
		};

		struct is_word
		{
			static bool	eval(char c);
		};

		struct is_symbol
		{
			static bool eval(char c);
		};

		class symbol
		{
			char	c_;

		public:

			explicit symbol(char c)
				: c_(c)
			{
			}

			friend Parser & operator >> (Parser & parser, symbol const & sym)
			{
				Token token = parser.next_token();

				expect(token, Token::Symbol);
				LV_ENSURE(*token.begin == sym.c_, std::string("symbol expected:") + sym.c_ + " given:" + token.str());

				return parser;
			}
		};

		class literal
		{
			char const *	lit_;

		public:

			explicit literal(char const * lit)
				: lit_(lit)
			{
			}

			friend Parser & operator >> (Parser & parser, literal const & lit)
			{
				Token token = parser.next_token();
				expect(token, Token::Literal);

				LV_ENSURE(token == lit.lit_, std::string("literal expected:") + lit.lit_);

				return parser;
			}
		};


		// unknown_tag

		template<class T>
		void	load_impl(Parser & parser, T & t, unknown_tag)
		{
			parser >> symbol('{');

			unsigned int ver = 0;

			if (parser.read_if(VersionKey.c_str()))
			{
				parser >> symbol('=') >> ver;
			}

			boost::serialization::serialize_adl(parser, t, ver);

			parser >> symbol('}');
		}


		// primitive_tag

		void	assign(std::string & t, Token const & token);

		void	assign(bool & t, Token const & token);

		template<class T>
		void	assign(T & t, Token const & token)
		{
			expect(token, Token::Number);

			size_t const size = 128;
			LV_ENSURE(token.size() < size, "too long number:" + token.str());

			char buf[size];
			memcpy(buf, token.begin, token.size());
			buf[token.size()] = '\0';

			char * end = nullptr;
			double val = strtod(buf, &end);

			LV_ENSURE(buf + token.size() == end, "invalid number:" + token.str());

			t = static_cast<T>(val);
		}

		template<class T>
		void	load_impl(Parser & parser, T & t, primitive_tag)
		{
			assign(t, parser.next_token());
		}


		// enum_tag

		template<class T>
		void	load_impl(Parser & parser, T & t, enum_tag)
		{
			int v;
			parser >> v;

			t = static_cast<T>(v);
		}


		// sequence_tag

		template<class T>
		void	load_impl(Parser & parser, T & t, sequence_tag)
		{
			parser >> symbol('{');

			int index = 1;

			while (!parser.read_if('}'))
			{
				typename boost::range_value<T>::type item;

				load_item(parser, index++, item);
				lv::insert(t, std::move(item));
			}
		}
	}

	template<class T>
	void	load_item(Parser & parser, int index, T & t)
	{
		parser >> t;
	}

	template<class T>
	void	load(Parser & parser, boost::serialization::nvp<T> const & t)
	{
		parser >> detail::literal(t.name()) >> detail::symbol('=') >> t.value();
	}

	template<class T>
	void	load(Parser & parser, T & t)
	{
		detail::load_impl(parser, t, object_tag_t<T>());
	}


}
