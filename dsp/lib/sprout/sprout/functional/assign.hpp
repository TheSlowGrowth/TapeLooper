/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FUNCTIONAL_ASSIGN_HPP
#define SPROUT_FUNCTIONAL_ASSIGN_HPP

#include <utility>
#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/functional/transparent.hpp>

namespace sprout {
	//
	// assign
	//
	template<typename T = void>
	struct assign;
	template<>
	struct assign<void>
		: public sprout::transparent<>
	{
	public:
		template<typename T, typename U>
		SPROUT_CONSTEXPR decltype(std::declval<T>() = std::declval<U>())
		operator()(T&& x, U&& y)
		const SPROUT_NOEXCEPT_IF_EXPR(std::declval<T>() = std::declval<U>())
		{
			return SPROUT_FORWARD(T, x) = SPROUT_FORWARD(U, y);
		}
	};
}	// namespace sprout

#endif	// #ifndef SPROUT_FUNCTIONAL_ASSIGN_HPP
