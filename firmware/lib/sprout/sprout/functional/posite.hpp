/*=============================================================================
  Copyright (c) 2011 RiSK (sscrisk)
  https://github.com/sscrisk/CEL---ConstExpr-Library

  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FUNCTIONAL_POSITE_HPP
#define SPROUT_FUNCTIONAL_POSITE_HPP

#include <utility>
#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/functional/transparent.hpp>

namespace sprout {

	// 20.8.4 Arithmetic operations
	template<typename T = void>
	struct posite {
	public:
		typedef T argument_type;
		typedef T result_type;
	public:
		SPROUT_CONSTEXPR T operator()(T const& x) const {
				return +x;
		}
	};

	template<>
	struct posite<void>
		: public sprout::transparent<>
	{
	public:
		template<typename T>
		SPROUT_CONSTEXPR decltype(+std::declval<T>())
		operator()(T&& x)
		const SPROUT_NOEXCEPT_IF_EXPR(+std::declval<T>())
		{
			return +SPROUT_FORWARD(T, x);
		}
	};
}	// namespace sprout

#endif	// #ifndef SPROUT_FUNCTIONAL_POSITE_HPP
