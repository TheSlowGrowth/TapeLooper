/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ADAPTOR_DROPPED_HPP
#define SPROUT_RANGE_ADAPTOR_DROPPED_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/iterator/next.hpp>
#include <sprout/range/adaptor/detail/adapted_range_default.hpp>
#include <sprout/type_traits/lvalue_reference.hpp>
#include <sprout/utility/lvalue_forward.hpp>
#include HDR_ALGORITHM_MIN_MAX_SSCRISK_CEL_OR_SPROUT

namespace sprout {
	namespace adaptors {
		//
		// drop_holder
		//
		template<typename Difference>
		class drop_holder {
		public:
			typedef Difference difference_type;
		private:
			difference_type distance_;
		public:
			explicit SPROUT_CONSTEXPR drop_holder(difference_type distance)
				: distance_(distance)
			{}
			SPROUT_CONSTEXPR difference_type const& distance() const {
				return distance_;
			}
		};

		//
		// dropped_forwarder
		//
		class dropped_forwarder {
		public:
			template<typename Difference>
			SPROUT_CONSTEXPR sprout::adaptors::drop_holder<Difference>
			operator()(Difference distance) const {
				return sprout::adaptors::drop_holder<Difference>(distance);
			}
		};

		//
		// dropped
		//
		namespace {
			SPROUT_STATIC_CONSTEXPR sprout::adaptors::dropped_forwarder dropped = {};
		}	// anonymous-namespace

		//
		// operator|
		//
		template<typename Range, typename Difference>
		inline SPROUT_CONSTEXPR sprout::adaptors::detail::adapted_range_default<
			typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type
		>
		operator|(Range&& lhs, sprout::adaptors::drop_holder<Difference> const& rhs) {
			typedef typename sprout::container_traits<
				typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type
			>::difference_type difference_type;
			return sprout::adaptors::detail::adapted_range_default<
				typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type
			>(
				sprout::next(
					sprout::begin(sprout::lvalue_forward<Range>(lhs)),
					NS_SSCRISK_CEL_OR_SPROUT::min<difference_type>(rhs.distance(), sprout::size(sprout::lvalue_forward<Range>(lhs)))
					),
				sprout::end(sprout::lvalue_forward<Range>(lhs))
				);
		}
	}	// namespace adaptors
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ADAPTOR_DROPPED_HPP
