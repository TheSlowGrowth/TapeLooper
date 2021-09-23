/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPOST_EFFECTS_NOISE_GATED_HPP
#define SPROUT_COMPOST_EFFECTS_NOISE_GATED_HPP

#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/range/adaptor/transformed.hpp>

namespace sprout {
	namespace compost {
		namespace detail {
			template<typename Value, typename T>
			SPROUT_CONSTEXPR T
			noise_gate_value(T const& x, Value const& threshold) {
				return x <= threshold && x >= -threshold ? 0
					: x
					;
			}
		}	// namespace detail
		//
		// noise_gate_value
		//
		template<typename Value, typename T = void>
		struct noise_gate_value {
		public:
			typedef Value value_type;
			typedef T argument_type;
			typedef T result_type;
		private:
			value_type threshold_;
		public:
			explicit SPROUT_CONSTEXPR noise_gate_value(Value const& threshold)
				: threshold_(threshold)
			{}
			SPROUT_CONSTEXPR result_type
			operator()(T const& x) const {
				return sprout::compost::detail::noise_gate_value(x, threshold_);
			}
		};
		template<typename Value>
		struct noise_gate_value<Value, void> {
		public:
			typedef Value value_type;
		private:
			value_type threshold_;
		public:
			explicit SPROUT_CONSTEXPR noise_gate_value(Value const& threshold)
				: threshold_(threshold)
			{}
			template<typename T>
			SPROUT_CONSTEXPR T
			operator()(T const& x) const {
				return sprout::compost::detail::noise_gate_value(x, threshold_);
			}
		};

		namespace effects {
			//
			// noise_gate_holder
			//
			template<typename T>
			class noise_gate_holder {
			public:
				typedef T value_type;
			private:
				value_type threshold_;
			public:
				SPROUT_CONSTEXPR noise_gate_holder() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
				noise_gate_holder(noise_gate_holder const&) = default;
				explicit SPROUT_CONSTEXPR noise_gate_holder(value_type const& threshold)
					: threshold_(threshold)
				{}
				SPROUT_CONSTEXPR value_type const& threshold() const {
					return threshold_;
				}
			};


			//
			// noise_gated_forwarder
			//
			class noise_gated_forwarder {
			public:
				template<typename T>
				SPROUT_CONSTEXPR sprout::compost::effects::noise_gate_holder<T>
				operator()(T const& threshold) const {
					return sprout::compost::effects::noise_gate_holder<T>(threshold);
				}
			};

			//
			// noise_gated
			//
			namespace {
				SPROUT_STATIC_CONSTEXPR sprout::compost::effects::noise_gated_forwarder noise_gated = {};
			}	// anonymous-namespace

			//
			// operator|
			//
			template<typename Range, typename T>
			inline SPROUT_CONSTEXPR auto
			operator|(Range&& lhs, sprout::compost::effects::noise_gate_holder<T> const& rhs)
			-> decltype(
				SPROUT_FORWARD(Range, lhs)
					| sprout::adaptors::transformed(sprout::compost::noise_gate_value<T>(rhs.threshold()))
				)
			{
				return SPROUT_FORWARD(Range, lhs)
					| sprout::adaptors::transformed(sprout::compost::noise_gate_value<T>(rhs.threshold()))
					;
			}
		}	// namespace effects

		using sprout::compost::effects::noise_gated;
	}	// namespace compost
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPOST_EFFECTS_NOISE_GATED_HPP
