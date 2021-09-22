/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ADAPTOR_DEEP_COPIED_HPP
#define SPROUT_RANGE_ADAPTOR_DEEP_COPIED_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/utility/forward.hpp>

namespace sprout {
	namespace adaptors {
		//
		// deep_copied_forwarder
		//
		class deep_copied_forwarder {};

		//
		// deep_copied
		//
		namespace {
			SPROUT_STATIC_CONSTEXPR sprout::adaptors::deep_copied_forwarder deep_copied = {};
		}	// anonymous-namespace

		//
		// operator|
		//
		template<typename Range>
		inline SPROUT_CONSTEXPR typename sprout::container_construct_traits<
			typename std::remove_reference<Range>::type
		>::copied_type
		operator|(Range&& lhs, sprout::adaptors::deep_copied_forwarder const&) {
			return sprout::deep_copy(SPROUT_FORWARD(Range, lhs));
		}
	}	// namespace adaptors
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ADAPTOR_DEEP_COPIED_HPP
