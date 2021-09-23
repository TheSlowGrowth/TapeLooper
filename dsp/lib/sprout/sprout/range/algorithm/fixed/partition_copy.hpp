/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ALGORITHM_FIXED_PARTITION_COPY_HPP
#define SPROUT_RANGE_ALGORITHM_FIXED_PARTITION_COPY_HPP

#include <sprout/config.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/iterator/type_traits/is_iterator_of.hpp>
#include <sprout/type_traits/enabler_if.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/algorithm/fixed/partition_copy.hpp>

namespace sprout {
	namespace range {
		namespace fixed {
			//
			// partition_copy
			//
			template<
				typename InputRange, typename Result, typename Predicate,
				typename sprout::enabler_if<!sprout::is_iterator_outputable<Result>::value>::type = sprout::enabler
			>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
			partition_copy(InputRange const& rng, Result const& result, Predicate pred) {
				return sprout::fixed::partition_copy(sprout::begin(rng), sprout::end(rng), result, pred);
			}

			template<typename Result, typename InputRange, typename Predicate>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
			partition_copy(InputRange const& rng, Predicate pred) {
				return sprout::fixed::partition_copy<Result>(sprout::begin(rng), sprout::end(rng), pred);
			}
		}	// namespace fixed

		using sprout::range::fixed::partition_copy;
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ALGORITHM_FIXED_PARTITION_COPY_HPP
