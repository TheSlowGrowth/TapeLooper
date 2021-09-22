/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FUNCTIONAL_HASH_TO_HASH_HPP
#define SPROUT_FUNCTIONAL_HASH_TO_HASH_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/functional/hash/hash_fwd.hpp>

namespace sprout {
	//
	// to_hash
	//
	//	effect:
	//		sprout::hash_value_traits<typename std::remove_reference<T>::type>::hash_value(v)
	//		[default]
	//			ADL callable hash_value(v) -> hash_value(v)
	//			[default]
	//				T is Arithmetic || Enum || Pointer || Array -> implementation-defined
	//				otherwise -> std::hash<T>()(v)
	//
	template<typename T>
	inline SPROUT_CONSTEXPR std::size_t
	to_hash(T&& v) {
		return sprout::hash_value_traits<typename std::remove_reference<T>::type>::hash_value(v);
	}
}	// namespace sprout

#include <sprout/functional/hash/hash_value_traits.hpp>

#endif	// #ifndef SPROUT_FUNCTIONAL_HASH_TO_HASH_HPP
