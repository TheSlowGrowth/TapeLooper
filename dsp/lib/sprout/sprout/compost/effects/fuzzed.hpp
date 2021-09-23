/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPOST_EFFECTS_FUZZED_HPP
#define SPROUT_COMPOST_EFFECTS_FUZZED_HPP

#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/range/adaptor/transformed.hpp>
#include <sprout/compost/effects/rectified.hpp>
#include <sprout/compost/effects/distorted.hpp>

namespace sprout {
	namespace compost {
		namespace effects {
			//
			// fuzz_holder
			//
			template<typename T>
			class fuzz_holder {
			public:
				typedef T value_type;
			private:
				value_type gain_;
				value_type level_;
			public:
				SPROUT_CONSTEXPR fuzz_holder() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
				fuzz_holder(fuzz_holder const&) = default;
				SPROUT_CONSTEXPR fuzz_holder(value_type const& gain, value_type const& level)
					: gain_(gain), level_(level)
				{}
				SPROUT_CONSTEXPR value_type const& gain() const {
					return gain_;
				}
				SPROUT_CONSTEXPR value_type const& level() const {
					return level_;
				}
			};

			//
			// fuzzed_forwarder
			//
			class fuzzed_forwarder {
			public:
				template<typename T>
				SPROUT_CONSTEXPR sprout::compost::effects::fuzz_holder<T>
				operator()(T const& gain, T const& level) const {
					return sprout::compost::effects::fuzz_holder<T>(gain, level);
				}
			};

			//
			// fuzzed
			//
			namespace {
				SPROUT_STATIC_CONSTEXPR sprout::compost::effects::fuzzed_forwarder fuzzed = {};
			}	// anonymous-namespace

			//
			// operator|
			//
			template<typename Range, typename T>
			inline SPROUT_CONSTEXPR auto
			operator|(Range&& lhs, sprout::compost::effects::fuzz_holder<T> const& rhs)
			-> decltype(
				SPROUT_FORWARD(Range, lhs)
					| sprout::compost::effects::rectified
					| sprout::compost::effects::distorted(rhs.gain(), rhs.level())
				)
			{
				return SPROUT_FORWARD(Range, lhs)
					| sprout::compost::effects::rectified
					| sprout::compost::effects::distorted(rhs.gain(), rhs.level())
					;
			}
		}	// namespace effects

		using sprout::compost::effects::fuzzed;
	}	// namespace compost
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPOST_EFFECTS_FUZZED_HPP
