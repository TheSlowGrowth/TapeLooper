/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FUNCTIONAL_POLYMORPHIC_MEMBER_HPP
#define SPROUT_FUNCTIONAL_POLYMORPHIC_MEMBER_HPP

#include <sprout/config.hpp>
#include <sprout/functional/member.hpp>

namespace sprout {
	//
	// member_t
	// member_
	//
	typedef sprout::member<> member_t;
	namespace {
		SPROUT_STATIC_CONSTEXPR sprout::member_t member_ = {};
	}	// anonymous-namespace
}	// namespace sprout

#endif	// #ifndef SPROUT_FUNCTIONAL_POLYMORPHIC_MEMBER_HPP
