/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FIXED_CONTAINER_FIT_SIZE_HPP
#define SPROUT_FIXED_CONTAINER_FIT_SIZE_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/container/container_traits.hpp>
#include <sprout/container/container_fitness_traits.hpp>
#include <sprout/utility/forward.hpp>

namespace sprout {
	//
	// fit_size
	//
	template<typename Container>
	inline SPROUT_CONSTEXPR typename sprout::container_traits<typename std::remove_reference<Container>::type>::difference_type
	fit_size(Container&& cont, typename sprout::container_traits<typename std::remove_reference<Container>::type>::difference_type size) {
		typedef typename std::remove_reference<Container>::type container_type;
		return sprout::container_fitness_traits<container_type>::fit_size(SPROUT_FORWARD(Container, cont), size);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_FIXED_CONTAINER_FIT_SIZE_HPP
