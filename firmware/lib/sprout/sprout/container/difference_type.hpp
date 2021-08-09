/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_CONTAINER_DIFFERENCE_TYPE_HPP
#define SPROUT_CONTAINER_DIFFERENCE_TYPE_HPP

#include <sprout/config.hpp>
#include <sprout/container/container_traits.hpp>

namespace sprout {
	namespace containers {
		//
		// difference_type
		//
		template<typename Container>
		struct difference_type {
		public:
			typedef typename sprout::container_traits<Container>::difference_type type;
		};
	}	// namespace containers
}	// namespace sprout

#endif	// #ifndef SPROUT_CONTAINER_DIFFERENCE_TYPE_HPP
