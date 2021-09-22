/*=============================================================================
  Copyright (c) 2011 RiSK (sscrisk)
  https://github.com/sscrisk/CEL---ConstExpr-Library

  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ALGORITHM_FIND_HPP
#define SPROUT_RANGE_ALGORITHM_FIND_HPP

#include <sprout/config.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/range/range_return.hpp>
#include <sprout/algorithm/find.hpp>

namespace sprout {
	namespace range {

		// 25.2.5 Find
		template<typename InputRange, typename T>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<InputRange>::type
		find(InputRange&& range, T const& value) {
			return sprout::range::range_return<InputRange>::pack(
				sprout::find(
					sprout::begin(SPROUT_FORWARD(InputRange, range)),
					sprout::end(SPROUT_FORWARD(InputRange, range)),
					value
					),
				SPROUT_FORWARD(InputRange, range)
				);
		}

		template<sprout::range::range_return_value RetV, typename InputRange, typename T>
		inline SPROUT_CONSTEXPR typename sprout::range::range_return<InputRange, RetV>::type
		find(InputRange&& range, T const& value) {
			return sprout::range::range_return<InputRange, RetV>::pack(
				sprout::find(
					sprout::begin(SPROUT_FORWARD(InputRange, range)),
					sprout::end(SPROUT_FORWARD(InputRange, range)),
					value
					),
				SPROUT_FORWARD(InputRange, range)
				);
		}
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ALGORITHM_FIND_HPP
