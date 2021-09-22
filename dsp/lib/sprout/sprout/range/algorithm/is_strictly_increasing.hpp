/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ALGORITHM_IS_STRICTLY_INCREASING_HPP
#define SPROUT_RANGE_ALGORITHM_IS_STRICTLY_INCREASING_HPP

#include <sprout/config.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/is_strictly_increasing.hpp>

namespace sprout {
	namespace range {
		//
		// is_strictly_increasing
		//
		template<typename ForwardRange>
		inline SPROUT_CONSTEXPR bool
		is_strictly_increasing(ForwardRange const& range) {
			return sprout::is_strictly_increasing(sprout::begin(range), sprout::end(range));
		}
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ALGORITHM_IS_STRICTLY_INCREASING_HPP
