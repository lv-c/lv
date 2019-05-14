#include <lv/Config.hpp>
#include <lv/LuaArchive/BinaryObject.hpp>
#include <lv/LuaArchive/ISerializer.hpp>
#include <lv/LuaArchive/PlainISerializer.hpp>
#include <lv/Ensure.hpp>

#include <boost/serialization/binary_object.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/istream_iterator.hpp>


namespace lv::lua::archive
{
	void	save(std::ostream & os, boost::serialization::binary_object const & t, size_t level)
	{
		boost::archive::iterators::ostream_iterator<char> it(os);
		*it++ = '\'';

		using base64 = boost::archive::iterators::insert_linebreaks<
			boost::archive::iterators::base64_from_binary<
				boost::archive::iterators::transform_width<char const *, 6, 8>
			>,
			76
		>;

		char const * p = static_cast<char const *>(t.m_t);
		std::copy(base64(p), base64(p + t.m_size), it);

		constexpr size_t block_num = 3;
		size_t pad_num = (block_num - t.m_size % block_num) % block_num;

		for (size_t i = 0; i < pad_num; ++i)
		{
			*it++ = '=';
		}

		*it++ = '\'';
	}


	namespace detail
	{
		void	load_binary(std::string_view str, boost::serialization::binary_object const & t)
		{
			using base64 = boost::archive::iterators::transform_width<
				boost::archive::iterators::binary_from_base64<
					boost::archive::iterators::remove_whitespace<
						std::string_view::iterator
					>
				>, 8, 6, char
			>;

			base64 it(str.begin()), end(str.end());
			char * p = static_cast<char *>(const_cast<void *>(t.m_t));

			size_t count = t.m_size;
			while (count-- > 0)
			{
				LV_ENSURE(it != end, "unmatched base64 size");
				*p++ = *it++;
			}
		}
	}


	void	load(luabind::object const & obj, boost::serialization::binary_object const & t)
	{
		expect_obj_type(obj, LUA_TSTRING);

		lua_State * L = obj.interpreter();
		obj.push(L);
		luabind::detail::stack_pop pop(L, 1);

		int const index = -1;
		size_t len;
		char const * str = lua_tolstring(L, index, &len);

		detail::load_binary(std::string_view(str, len), t);
	}


	void	load(Parser & parser, boost::serialization::binary_object const & t)
	{
		std::string str;
		parser >> str;

		detail::load_binary(str, t);
	}
}