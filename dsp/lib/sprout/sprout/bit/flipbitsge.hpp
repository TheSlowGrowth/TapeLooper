/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_BIT_FLIPBITSGE_HPP
#define SPROUT_BIT_FLIPBITSGE_HPP

#include <type_traits>
#include <sprout/config.hpp>

namespace sprout {
	//
	// flipbitsge
	//
	template<typename Integral>
	inline SPROUT_CONSTEXPR typename std::enable_if<
		std::is_integral<Integral>::value,
		Integral
	>::type
	flipbitsge(Integral x, int b) SPROUT_NOEXCEPT {
		return x ^ ~((Integral(1) << b) - 1);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_BIT_FLIPBITSGE_HPP
