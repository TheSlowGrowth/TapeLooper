/*=============================================================================
  Copyright (c) 2011 RiSK (sscrisk)
  https://github.com/sscrisk/CEL---ConstExpr-Library

  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ALGORITHM_ADJACENT_FIND_HPP
#define SPROUT_RANGE_ALGORITHM_ADJACENT_FIND_HPP

#include <sprout/config.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/range/range_return.hpp>
#include <sprout/algorithm/adjacent_find.hpp>

namespace sprout {
	namespace range {

		// 25.2.8 Adjacent find
		template<typename ForwardRange>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<ForwardRange>::type
		adjacent_find(ForwardRange&& range) {
			return sprout::range::range_return<ForwardRange>::pack(
				sprout::adjacent_find(
					sprout::begin(SPROUT_FORWARD(ForwardRange, range)),
					sprout::end(SPROUT_FORWARD(ForwardRange, range))
					),
				SPROUT_FORWARD(ForwardRange, range)
				);
		}

		template<typename ForwardRange, typename BinaryPredicate>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<ForwardRange>::type
		adjacent_find(ForwardRange&& range, BinaryPredicate pred) {
			return sprout::range::range_return<ForwardRange>::pack(
				sprout::adjacent_find(
					sprout::begin(SPROUT_FORWARD(ForwardRange, range)),
					sprout::end(SPROUT_FORWARD(ForwardRange, range)),
					pred
					),
				SPROUT_FORWARD(ForwardRange, range)
				);
		}

		template<sprout::range::range_return_value RetV, typename ForwardRange>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<ForwardRange, RetV>::type
		adjacent_find(ForwardRange&& range) {
			return sprout::range::range_return<ForwardRange, RetV>::pack(
				sprout::adjacent_find(
					sprout::begin(SPROUT_FORWARD(ForwardRange, range)),
					sprout::end(SPROUT_FORWARD(ForwardRange, range))
					),
				SPROUT_FORWARD(ForwardRange, range)
				);
		}

		template<sprout::range::range_return_value RetV, typename ForwardRange, typename BinaryPredicate>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<ForwardRange, RetV>::type
		adjacent_find(ForwardRange&& range, BinaryPredicate pred) {
			return sprout::range::range_return<ForwardRange, RetV>::pack(
				sprout::adjacent_find(
					sprout::begin(SPROUT_FORWARD(ForwardRange, range)),
					sprout::end(SPROUT_FORWARD(ForwardRange, range)),
					pred
					),
				SPROUT_FORWARD(ForwardRange, range)
				);
		}
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ALGORITHM_ADJACENT_FIND_HPP
