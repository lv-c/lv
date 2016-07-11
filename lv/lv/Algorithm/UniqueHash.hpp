// *********************************************************************
//  UniqueHash   version:  1.0   ¡¤  date: 09/23/2008
//  --------------------------------------------------------------------
//  unique hash for ( a given set of objects
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_UNIQUEHASH_HPP
#define LV_UNIQUEHASH_HPP

#include <climits>

#include <boost/functional/hash.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/range/concepts.hpp>
#include <boost/unordered_map.hpp>
#include <boost/utility/enable_if.hpp>

#include <lv/IntType.hpp>
#include <lv/Exception.hpp>

namespace lv { namespace unique_hash {


	namespace detail
	{
		/**
		 * returns the maximum unsigned value of type boost::make_unsigned<Key>::type
		 */
		template<typename Key>
		inline typename boost::make_unsigned<Key>::type max_value()
		{
			// std::numeric_limits<T const>::max() == 0   ...(T = char , short ...)
			// so we have to remove the const qualifier
			return std::numeric_limits<
				typename boost::make_unsigned<
					typename boost::remove_const<Key>::type
				>::type
			>::max();
		}


		/**
		 * compute the hash value of a range type (models boost::SinglePassRangeConcept concept)
		 */
		template<typename Value>
		typename boost::disable_if<boost::is_arithmetic<Value>, std::size_t>::type 
			hash_impl(uint32 seed, Value const & value)
		{
			BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<Value>));	// concept check

			size_t result(seed);
			boost::hash_range(result, std::begin(value), std::end(value));
			return result;
		}

		/// c string
		inline std::size_t hash_impl(uint32 seed, char const * value)
		{
			size_t result(seed);
			for (char const * p = value; *p != '\0'; ++p)
				boost::hash_combine(result, *p);
		
			return result;
		}

		inline std::size_t hash_impl(uint32 seed, wchar_t const * value)
		{
			size_t result(seed);
			for (wchar_t const * p = value; *p != '\0'; ++p)
				boost::hash_combine(result, *p);

			return result;
		}

		/**
		 * compute the hash value of an arithmetic type. @a seed is ignored
		 */
		template<typename Value>
		typename boost::enable_if<boost::is_arithmetic<Value>, std::size_t>::type
			hash_impl(uint32 seed, Value value)
		{
			return boost::hash_value(value);
		}

	}

	/**
	 * compute the hash code of @a value with @a seed
	 */
	template<typename Key, typename Value>
	Key hash(uint32 seed, Value const & value)
	{
		static_assert(boost::is_arithmetic<Key>::value, "value type should be arithmetic");
		// boost::hash_range may return different values on platforms where size_t has different sizes.
		// if this assertion fails, we may have to implement our own hash functions.
		static_assert(sizeof(std::size_t) == 4, "currently support 32bit platform only");

		return detail::hash_impl(seed, value) % detail::max_value<Key>();
	}



	DEFINE_EXCEPTION_MSG(DuplicatedObjects, std::runtime_error);
	DEFINE_EXCEPTION_MSG(NoSeedFound, std::runtime_error);

	/**
	 * Find a hash seed with which the function @a lv::hash will generate
	 * a unique hash code for ( each value in @a values.
	 * @exception DuplicatedObjects if @a values contains duplicated objects 
	 * @exception NoSeedFound if there's no seed found that meets the requirements.
	 */
	template<typename Key, class Values>
	uint32 seed(Values const & values)
	{
		BOOST_CONCEPT_ASSERT((boost::SinglePassRangeConcept<Values>));	// concept check

		typedef typename boost::range_value<Values>::type value_t;
		typedef boost::unordered_map<Key, value_t>	map_t;

		uint32 max_seed = detail::max_value<uint32>();
		
		for (uint32 i = 0; i < max_seed; ++i)
		{
			map_t map;
			bool ok = true;
			for (value_t const & v : values)
			{
				Key key = hash<Key>(i, v);
				map_t::iterator it = map.find(key);

				if (it != map.end())
				{
					if (it->second == v)		// duplicated
					{
						throw DuplicatedObjects();
					}

					ok = false;
					break;
				}
				else
				{
					map.insert(std::make_pair(key, v));
				}
			}
			
			if (ok)
			{
				return i;
			}
		}

		throw NoSeedFound();
	}


} }

#endif