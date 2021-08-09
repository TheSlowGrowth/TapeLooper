/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP
#define SPROUT_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/type_traits/detail/type_traits_wrapper.hpp>

namespace sprout {
	//
	// is_constructible
	//
	template<typename T, typename... Args>
	struct is_constructible
		: public sprout::detail::type_traits_wrapper<std::is_constructible<T, Args...> >
	{};

#if SPROUT_USE_VARIABLE_TEMPLATES
	template<typename T, typename... Args>
	SPROUT_STATIC_CONSTEXPR bool is_constructible_v = sprout::is_constructible<T, Args...>::value;
#endif	// #if SPROUT_USE_VARIABLE_TEMPLATES
}	// namespace sprout

#endif	// #ifndef SPROUT_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP