/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_NUMERIC_FIT_EXCLUSIVE_SCAN_HPP
#define SPROUT_RANGE_NUMERIC_FIT_EXCLUSIVE_SCAN_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/fit/results.hpp>
#include <sprout/numeric/fit/exclusive_scan.hpp>

namespace sprout {
	namespace range {
		namespace fit {
			//
			// exclusive_scan
			//
			template<typename InputRange, typename Result, typename T>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			exclusive_scan(InputRange const& rng, Result const& result, T init) {
				return sprout::fit::exclusive_scan(sprout::begin(rng), sprout::end(rng), result, init);
			}
			template<typename InputRange, typename Result, typename T, typename BinaryOperation>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			exclusive_scan(InputRange const& rng, Result const& result, T init, BinaryOperation binary_op) {
				return sprout::fit::exclusive_scan(sprout::begin(rng), sprout::end(rng), result, init, binary_op);
			}

			template<typename Result, typename InputRange, typename T>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			exclusive_scan(InputRange const& rng, T init) {
				return sprout::fit::exclusive_scan<Result>(sprout::begin(rng), sprout::end(rng), init);
			}
			template<typename Result, typename InputRange, typename T, typename BinaryOperation>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			exclusive_scan(InputRange const& rng, T init, BinaryOperation binary_op) {
				return sprout::fit::exclusive_scan<Result>(sprout::begin(rng), sprout::end(rng), init, binary_op);
			}
		}	// namespace fit
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_NUMERIC_FIT_EXCLUSIVE_SCAN_HPP
