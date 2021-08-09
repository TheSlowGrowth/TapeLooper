/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_SIZED_PIT_COMPARISON_HPP
#define SPROUT_SIZED_PIT_COMPARISON_HPP

#include <sprout/config.hpp>
#include <sprout/sized_pit/sized_pit.hpp>

namespace sprout {
	//
	// operator==
	// operator!=
	// operator<
	// operator>
	// operator<=
	// operator>=
	//
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator==(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return lhs.front() == rhs.front();
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator!=(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return !(lhs == rhs);
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator<(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return lhs.front() < rhs.front();
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator>(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return rhs < lhs;
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator<=(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return !(rhs < lhs);
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR bool
	operator>=(sprout::sized_pit<Container> const& lhs, sprout::sized_pit<Container> const& rhs) {
		return !(lhs < rhs);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_SIZED_PIT_COMPARISON_HPP
