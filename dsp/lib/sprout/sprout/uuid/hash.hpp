/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_UUID_HASH_HPP
#define SPROUT_UUID_HASH_HPP

#include <functional>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/uuid/uuid.hpp>
#include <sprout/functional/hash.hpp>

namespace sprout {
	//
	// hash_value
	//
	inline SPROUT_CONSTEXPR std::size_t
	hash_value(sprout::uuids::uuid const& v) {
		return sprout::hash_range(v);
	}
}	// namespace sprout

namespace std {
#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wmismatched-tags"
#endif
	//
	// hash
	//
	template<>
	struct hash<sprout::uuids::uuid>
		: public sprout::hash<sprout::uuids::uuid>
	{};
#if defined(__clang__)
#	pragma clang diagnostic pop
#endif
}	// namespace std

#endif	// #ifndef SPROUT_UUID_HASH_HPP
