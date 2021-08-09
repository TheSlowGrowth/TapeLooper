/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_VARIANT_APPLY_VISITOR_HPP
#define SPROUT_VARIANT_APPLY_VISITOR_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/variant/visitor_result.hpp>

namespace sprout {
	//
	// apply_visitor
	//
	template<typename Visitor, typename Visitable>
	inline SPROUT_CONSTEXPR typename sprout::visitor_result<
		typename std::remove_reference<Visitor>::type,
		typename std::remove_reference<Visitable>::type
	>::type
	apply_visitor(Visitor&& visitor, Visitable&& visitable) {
		return SPROUT_FORWARD(Visitable, visitable).apply_visitor(SPROUT_FORWARD(Visitor, visitor));
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_VARIANT_APPLY_VISITOR_HPP
