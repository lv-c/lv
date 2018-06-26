#include <lv/LuaArchive/PlainISerializer.hpp>


namespace lv::lua::archive
{
	namespace detail
	{
		inline bool is_space::eval(char c)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case ',':
				return true;

			default:
				return false;
			}
		}

		inline bool is_number::eval(char c)
		{
			if ('0' <= c && c <= '9')
			{
				return true;
			}

			switch (c)
			{
			case '-':
			case '.':
				return true;

			default:
				return false;
			}
		}

		inline bool is_word::eval(char c)
		{
			return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_' || ('0' <= c && c <= '9'));
		}

		inline bool is_symbol::eval(char c)
		{
			switch (c)
			{
			case '{':
			case '}':
			case '=':
				return true;

			default:
				return false;
			}
		}

		uint8_t	hex_value(char c)
		{
			if (isdigit(c))
			{
				return c - '0';
			}
			else
			{
				return tolower(c) - 'a' + 10;
			}
		}

		char	read_hex(Token::iterator p, Token::iterator end)
		{
			uint8_t c = 0;

			for (int i = 0; i < 2; ++i, ++p)
			{
				LV_ENSURE(p != end && isxdigit(*p), "hexadecimal digit expected");

				c = (c << 4) + hex_value(*p);
			}

			return c;
		}

		void	assign(std::string & t, Token const & token)
		{
			expect(token, Token::String);

			Token::iterator p = token.begin;
			Token::iterator end = token.end;

			if (*p == '[')
			{
				p += 2;
				end -= 2;
			}
			else
			{
				++p;
				--end;
			}

			t.clear();
			t.reserve(end - p);

			while (p != end)
			{
				char c = *p;

				if (*p == '\\')
				{
					switch (* ++p)
					{
					case '\\':
					case '\'':
					case '\"':
						c = *p;
						break;

					case 'a':
						c = '\a';
						break;

					case 'b':
						c = '\b';
						break;

					case 'f':
						c = '\f';
						break;

					case 'n':
						c = '\n';
						break;

					case 'r':
						c = '\r';
						break;

					case 't':
						c = '\t';
						break;

					case 'v':
						c = '\v';
						break;

					case 'x':
						c = read_hex(p + 1, end);
						break;

					default:
						throw std::runtime_error(std::string("invalid escape sequence:") + *p);
					}
				}

				t.push_back(c);
				p++;
			}
		}

		void	assign(bool & t, Token const & token)
		{
			expect(token, Token::Literal);

			if (token == "true")
			{
				t = true;
			}
			else if (token == "false")
			{
				t = false;
			}
			else
			{
				throw std::runtime_error("invalid bool value:" + token.str());
			}
		}
	}

	char const * Token::name(Token::Type type)
	{
		switch (type)
		{
		case None:
			return "None";

		case Symbol:
			return "Symbol";

		case Number:
			return "Number";

		case String:
			return "String";

		case Literal:
			return "Literal";

		default:
			BOOST_ASSERT(false);
			return "";
		}
	}

	Parser::Parser(ConstBufferRef buf)
		: holder_(buf)
		, cur_(buf.begin())
		, end_(buf.end())
	{
	}

	Token Parser::next_token()
	{
		next(detail::is_space());

		LV_ENSURE(cur_ != end_, "unexpected end");

		iterator begin = cur_;
		Token::Type type = Token::None;

		char c = *cur_;

		if (c == '\'' || c == '"' || (c == '[' && test_and_forward(cur_ + 1, '[')))
		{
			cur_++;

			bool valid = false;

			while (cur_ != end_)
			{
				char new_c = *cur_++;

				if (new_c == '\\')
				{
					if (cur_ != end_)
					{
						cur_++;
					}
				}
				else if (new_c == c && c != '[')
				{
					valid = true;
					break;
				}
				else if (c == '[' && new_c == ']' && test_and_forward(cur_, ']'))
				{
					valid = true;
					break;
				}
			}

			LV_ENSURE(valid, "invalid string:" + std::string(cur_, end_));

			type = Token::String;
		}
		else if (c == '[' || c == ']')
		{
			cur_++;
			Token token = next_token();

			if (c == '[')
			{
				token.is_key = true;
			}

			return token;
		}
		else if (detail::is_symbol::eval(c))
		{
			cur_ ++;
			type = Token::Symbol;
		}
		else if (detail::is_number::eval(c))
		{
			next(detail::is_number());
			type = Token::Number;
		}
		else if (detail::is_word::eval(c))
		{
			next(detail::is_word());
			type = Token::Literal;
		}

		return Token(begin, cur_, type);
	}

	void Parser::rollback(Token const & token)
	{
		cur_ = token.begin;
	}

	bool Parser::test_and_forward(iterator it, char test)
	{
		if (it != end_ && *it == test)
		{
			cur_++;
			return true;
		}

		return false;
	}

	bool Parser::read_if(char c)
	{
		next(detail::is_space());

		if (cur_ != end_ && *cur_ == c)
		{
			cur_++;
			return true;
		}

		return false;
	}

	bool Parser::read_if(char const * lit)
	{
		next(detail::is_space());

		iterator old_p = cur_;

		try
		{
			Token token = next_token();

			if (token.type == Token::Literal && token == lit)
			{
				return true;
			}
		}
		catch (std::runtime_error const &)
		{
		}

		cur_ = old_p;
		return false;
	}

}