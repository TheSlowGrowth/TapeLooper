/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_UTILITY_MOVE_HPP
#define SPROUT_UTILITY_MOVE_HPP

#include <type_traits>
#include <sprout/config.hpp>

namespace sprout {
	//
	// move
	//
	template<typename T>
	inline SPROUT_CONSTEXPR typename std::remove_reference<T>::type&&
	move(T&& x) SPROUT_NOEXCEPT {
		return static_cast<typename std::remove_reference<T>::type&&>(x);
	}

	//
	// move_if_noexcept
	//
	template<typename T>
	inline SPROUT_CONSTEXPR typename std::conditional<
		!std::is_nothrow_move_constructible<T>::value && std::is_copy_constructible<T>::value,
		T const&,
		T&&
	>::type
	move_if_noexcept(T& x) SPROUT_NOEXCEPT {
		return sprout::move(x);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_UTILITY_MOVE_HPP
