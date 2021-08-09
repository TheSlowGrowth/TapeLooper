/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_CINTTYPES_ABS_HPP
#define SPROUT_CINTTYPES_ABS_HPP

#include <cstdint>
#include <sprout/config.hpp>

namespace sprout {
	inline SPROUT_CONSTEXPR std::intmax_t
	imaxabs(std::intmax_t j) {
		return j < 0 ? -j : j;
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_CINTTYPES_ABS_HPP
