/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_LIBS_RANDOM_TEST_GEOMETRIC_DISTRIBUTION_CPP
#define SPROUT_LIBS_RANDOM_TEST_GEOMETRIC_DISTRIBUTION_CPP

#include <sprout/random/geometric_distribution.hpp>
#include "./distribution_generic.hpp"
#include <testspr/tools.hpp>

namespace testspr {
	void random_geometric_distribution_test() {
		using namespace sprout;

		testspr::random_distribution_test_generic<sprout::random::geometric_distribution<> >();
	}
}	// namespace testspr

#ifndef TESTSPR_CPP_INCLUDE
#	define TESTSPR_TEST_FUNCTION testspr::random_geometric_distribution_test
#	include <testspr/include_main.hpp>
#endif

#endif	// #ifndef SPROUT_LIBS_RANDOM_TEST_GEOMETRIC_DISTRIBUTION_CPP
