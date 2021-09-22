/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_LIBS_ALGORITHM_TEST_UNIQUE_COPY_CPP
#define SPROUT_LIBS_ALGORITHM_TEST_UNIQUE_COPY_CPP

#include <sprout/algorithm/unique_copy.hpp>
#include <sprout/array.hpp>
#include <sprout/sub_array.hpp>
#include <sprout/container.hpp>
#include <testspr/tools.hpp>

namespace testspr {
	static void algorithm_unique_copy_test() {
		using namespace sprout;
		{
			SPROUT_STATIC_CONSTEXPR auto arr1 = array<int, 10>{{1, 1, 3, 3, 5, 5, 7, 7, 9, 9}};
			SPROUT_STATIC_CONSTEXPR auto arr2 = array<int, 10>{{}};
			SPROUT_STATIC_CONSTEXPR auto arr3 = array<int, 2>{{}};

			// "...Unique". in range [2 .. 8)
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr2
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 10>{{3, 5, 7, 0, 0, 0, 0, 0, 0, 0}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr2
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 3>{{3, 5, 7}}
					));
			}
			// "...Unique". in range [2 .. 8)
			// overrun from output range
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr3
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 2>{{3, 5}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr3
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 2>{{3, 5}}
					));
			}
			// "...Unique". in range [2 .. 8)
			// to sub range
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					sprout::sub(arr2, 2, 8)
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 6>{{3, 5, 7, 0, 0, 0}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(uniqued),
					array<int, 10>{{0, 0, 3, 5, 7, 0, 0, 0, 0, 0}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					sprout::sub(arr2, 2, 8)
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 3>{{3, 5, 7}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(uniqued),
					array<int, 10>{{0, 0, 3, 5, 7, 0, 0, 0, 0, 0}}
					));
			}
		}
		{
			SPROUT_STATIC_CONSTEXPR auto arr1 = array<int, 10>{{1, 1, 3, 3, 5, 5, 7, 7, 9, 9}};
			SPROUT_STATIC_CONSTEXPR auto arr2 = array<int, 10>{{}};
			SPROUT_STATIC_CONSTEXPR auto arr3 = array<int, 2>{{}};

			// "...Unique". in range [2 .. 8)
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr2,
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 10>{{3, 5, 7, 0, 0, 0, 0, 0, 0, 0}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr2,
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 3>{{3, 5, 7}}
					));
			}
			// "...Unique". in range [2 .. 8)
			// overrun from output range
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr3,
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 2>{{3, 5}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					arr3,
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 2>{{3, 5}}
					));
			}
			// "...Unique". in range [2 .. 8)
			// to sub range
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					sprout::sub(arr2, 2, 8),
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 6>{{3, 5, 7, 0, 0, 0}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(uniqued),
					array<int, 10>{{0, 0, 3, 5, 7, 0, 0, 0, 0, 0}}
					));
			}
			{
				SPROUT_STATIC_CONSTEXPR auto uniqued = sprout::fit::unique_copy(
					sprout::begin(arr1) + 2,
					sprout::begin(arr1) + 8,
					sprout::sub(arr2, 2, 8),
					testspr::equal_to<int>()
					);
				TESTSPR_BOTH_ASSERT(testspr::equal(
					uniqued,
					array<int, 3>{{3, 5, 7}}
					));
				TESTSPR_BOTH_ASSERT(testspr::equal(
					sprout::get_internal(uniqued),
					array<int, 10>{{0, 0, 3, 5, 7, 0, 0, 0, 0, 0}}
					));
			}
		}
	}
}	// namespace testspr

#ifndef TESTSPR_CPP_INCLUDE
#	define TESTSPR_TEST_FUNCTION testspr::algorithm_unique_copy_test
#	include <testspr/include_main.hpp>
#endif

#endif	// #ifndef SPROUT_LIBS_ALGORITHM_TEST_UNIQUE_COPY_CPP
