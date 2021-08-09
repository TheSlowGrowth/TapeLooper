/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ALGORITHM_ALL_OF_EQUAL_HPP
#define SPROUT_RANGE_ALGORITHM_ALL_OF_EQUAL_HPP

#include <sprout/config.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/all_of_equal.hpp>

namespace sprout {
	namespace range {
		//
		// all_of_equal
		//
		template<typename InputRange, typename T>
		inline SPROUT_CONSTEXPR bool
		all_of_equal(InputRange const& range, T const& value) {
			return sprout::all_of_equal(sprout::begin(range), sprout::end(range), value);
		}
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ALGORITHM_ALL_OF_EQUAL_HPP
