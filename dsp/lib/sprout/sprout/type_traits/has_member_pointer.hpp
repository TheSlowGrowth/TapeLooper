/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_TYPE_TRAITS_HAS_MEMBER_POINTER_HPP
#define SPROUT_TYPE_TRAITS_HAS_MEMBER_POINTER_HPP

#include <sprout/config.hpp>
#include <sprout/type_traits/dont_care.hpp>
#include <sprout/type_traits/detail/has_binary_op.hpp>
#include <sprout/preprocessor/empty.hpp>

namespace sprout {
	//
	// has_member_pointer
	//
	SPROUT_DETAIL_HAS_BINARY_OP_DECL(member_pointer, ->*, SPROUT_PP_EMPTY());

#if SPROUT_USE_VARIABLE_TEMPLATES
	template<typename T, typename U, typename R = sprout::dont_care>
	SPROUT_STATIC_CONSTEXPR bool has_member_pointer_v = sprout::has_member_pointer<T, U, R>::value;
#endif	// #if SPROUT_USE_VARIABLE_TEMPLATES
}	// namespace sprout

#endif	// #ifndef SPROUT_TYPE_TRAITS_HAS_MEMBER_POINTER_HPP
