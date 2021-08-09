/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_TYPE_TRAITS_HAS_DEREFERENCE_HPP
#define SPROUT_TYPE_TRAITS_HAS_DEREFERENCE_HPP

#include <sprout/config.hpp>
#include <sprout/type_traits/dont_care.hpp>
#include <sprout/type_traits/detail/has_pre_unary_op.hpp>

namespace sprout {
	//
	// has_dereference
	//
	SPROUT_DETAIL_HAS_PRE_UNARY_OP_DECL(dereference, *);

#if SPROUT_USE_VARIABLE_TEMPLATES
	template<typename T, typename R = sprout::dont_care>
	SPROUT_STATIC_CONSTEXPR bool has_dereference_v = sprout::has_dereference<T, R>::value;
#endif	// #if SPROUT_USE_VARIABLE_TEMPLATES
}	// namespace sprout

#endif	// #ifndef SPROUT_TYPE_TRAITS_HAS_DEREFERENCE_HPP
