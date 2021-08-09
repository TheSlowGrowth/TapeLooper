/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_LIBS_ALGORITHM_TEST_PARTITION_CPP
#define SPROUT_LIBS_ALGORITHM_TEST_PARTITION_CPP

#include <sprout/algorithm/partition.hpp>
#include <sprout/array.hpp>
#include <sprout/sub_array.hpp>
#include <sprout/container.hpp>
#include <testspr/tools.hpp>

namespace testspr {
	static void algorithm_partition_test() {
		using namespace sprout;
		{
			SPROUT_STATIC_CONSTEXPR auto arr1 = array<int, 10>{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};

			// partition (odd)
			{
				SPROUT_STATIC_CONSTEXPR auto partitioned = sprout::partition(
					arr1,
					testspr::is_odd<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					partitioned,
					array<int, 10>{{9, 7, 5, 3, 1, 2, 4, 6, 8, 10}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto partitioned = sprout::fit::partition(
					arr1,
					testspr::is_odd<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					partitioned,
					array<int, 5>{{9, 7, 5, 3, 1}}
					));
			}
			// partition (odd)
			// from sub range
			{
				SPROUT_STATIC_CONSTEXPR auto partitioned = sprout::partition(
					sprout::sub(arr1, 2, 8),
					testspr::is_odd<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					partitioned,
					array<int, 6>{{7, 5, 3, 4, 6, 8}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(partitioned),
					array<int, 10>{{1, 2, 7, 5, 3, 4, 6, 8, 9, 10}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto partitioned = sprout::fit::partition(
					sprout::sub(arr1, 2, 8),
					testspr::is_odd<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					partitioned,
					array<int, 3>{{7, 5, 3}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(partitioned),
					array<int, 10>{{1, 2, 7, 5, 3, 4, 6, 8, 9, 10}}
					));
			}
		}
	}
}	// namespace testspr

#ifndef TESTSPR_CPP_INCLUDE
#	define TESTSPR_TEST_FUNCTION testspr::algorithm_partition_test
#	include <testspr/include_main.hpp>
#endif

#endif	// #ifndef SPROUT_LIBS_ALGORITHM_TEST_PARTITION_CPP
