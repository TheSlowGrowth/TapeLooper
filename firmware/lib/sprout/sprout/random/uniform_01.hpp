/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANDOM_UNIFORM_01_HPP
#define SPROUT_RANDOM_UNIFORM_01_HPP

#include <iosfwd>
#include <istream>
#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/random/random_result.hpp>
#include <sprout/random/results.hpp>
#ifdef SPROUT_WORKAROUND_NOT_TERMINATE_RECURSIVE_CONSTEXPR_FUNCTION_TEMPLATE
#	include <sprout/workaround/recursive_function_template.hpp>
#endif

namespace sprout {
	namespace random {
		//
		// uniform_01
		//
		template<typename RealType = double>
		class uniform_01 {
		public:
			typedef RealType input_type;
			typedef RealType result_type;
		public:
			//
			// param_type
			//
			class param_type {
			public:
				typedef uniform_01 distribution_type;
			public:
				template<typename Elem, typename Traits>
				friend SPROUT_NON_CONSTEXPR std::basic_istream<Elem, Traits>& operator>>(
					std::basic_istream<Elem, Traits>& lhs,
					param_type&
					)
				{
					return lhs;
				}
				template<typename Elem, typename Traits>
				friend SPROUT_NON_CONSTEXPR std::basic_ostream<Elem, Traits>& operator<<(
					std::basic_ostream<Elem, Traits>& lhs,
					param_type const&
					)
				{
					return lhs;
				}
				friend SPROUT_CONSTEXPR bool operator==(param_type const&, param_type const&) SPROUT_NOEXCEPT {
					return true;
				}
				friend SPROUT_CONSTEXPR bool operator!=(param_type const& lhs, param_type const& rhs) SPROUT_NOEXCEPT {
					return !(lhs == rhs);
				}
			};
		private:
#ifdef SPROUT_WORKAROUND_NOT_TERMINATE_RECURSIVE_CONSTEXPR_FUNCTION_TEMPLATE
			template<int D, typename Engine, typename EngineResult, SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE(D)>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate_1(Engine const&, EngineResult const& rnd, result_type result) const {
				return result < result_type(1)
					? sprout::random::random_result<Engine, uniform_01>(result, rnd.engine(), *this)
					: generate<D + 1>(sprout::random::next(rnd), sprout::random::next(rnd)())
					;
			}
			template<int D, typename Engine, typename EngineResult, SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK(D)>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate_1(Engine const&, EngineResult const&, result_type) const {
				return sprout::throw_recursive_function_template_instantiation_exeeded();
			}
			template<int D = 16, typename Engine, typename EngineResult, SPROUT_RECURSIVE_FUNCTION_TEMPLATE_CONTINUE(D)>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate(Engine const& eng, EngineResult const& rnd) const {
				typedef typename Engine::result_type base_result;
				return generate_1<D + 1>(
					eng,
					rnd,
					result_type(sprout::random::result(rnd) - eng.min()) * (
						result_type(1) / (
							result_type(eng.max() - eng.min()) + result_type(
								sprout::numeric_limits<base_result>::is_integer ? 1 : 0
								)
							)
						)
					);
			}
			template<int D = 16, typename Engine, typename EngineResult, SPROUT_RECURSIVE_FUNCTION_TEMPLATE_BREAK(D)>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate(Engine const&, EngineResult const&) const {
				return sprout::throw_recursive_function_template_instantiation_exeeded();
			}
#else
			template<typename Engine, typename EngineResult>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate_1(Engine const&, EngineResult const& rnd, result_type result) const {
				return result < result_type(1)
					? sprout::random::random_result<Engine, uniform_01>(result, rnd.engine(), *this)
					: generate(sprout::random::next(rnd), sprout::random::next(rnd)())
					;
			}
			template<typename Engine, typename EngineResult>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01>
			generate(Engine const& eng, EngineResult const& rnd) const {
				typedef typename Engine::result_type base_result;
				return generate_1(
					eng,
					rnd,
					result_type(sprout::random::result(rnd) - eng.min()) * (
						result_type(1) / (
							result_type(eng.max() - eng.min()) + result_type(
								sprout::numeric_limits<base_result>::is_integer ? 1 : 0
								)
							)
						)
					);
			}
#endif
		public:
			SPROUT_CONSTEXPR uniform_01() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			uniform_01(uniform_01 const&) = default;
			explicit SPROUT_CONSTEXPR uniform_01(param_type const&) SPROUT_NOEXCEPT {}
			SPROUT_CONSTEXPR result_type min() const SPROUT_NOEXCEPT {
				return result_type(0);
			}
			SPROUT_CONSTEXPR result_type max() const SPROUT_NOEXCEPT {
				return result_type(1);
			}
			SPROUT_CXX14_CONSTEXPR void reset() SPROUT_NOEXCEPT {}
			SPROUT_CONSTEXPR param_type param() const SPROUT_NOEXCEPT {
				return param_type();
			}
			SPROUT_CXX14_CONSTEXPR void param(param_type const&) SPROUT_NOEXCEPT {}
			template<typename Engine>
			SPROUT_CXX14_CONSTEXPR result_type operator()(Engine& eng) const {
				for (; ; ) {
					typedef typename Engine::result_type base_result;
					result_type result = result_type(static_cast<base_result>(eng()) - eng.min()) * (
						result_type(1) / (
							result_type(eng.max() - eng.min()) + result_type(
								sprout::numeric_limits<base_result>::is_integer ? 1 : 0
								)
							)
						);
					if (result < result_type(1)) {
						return result;
					}
				}
			}
			template<typename Engine>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01> const operator()(Engine const& eng) const {
				return generate(eng, eng());
			}
			template<typename Engine>
			SPROUT_CXX14_CONSTEXPR result_type operator()(Engine& eng, param_type const&) const {
				return operator()(eng);
			}
			template<typename Engine>
			SPROUT_CONSTEXPR sprout::random::random_result<Engine, uniform_01> const operator()(Engine const& eng, param_type const&) const {
				return operator()(eng);
			}
			template<typename Elem, typename Traits>
			friend SPROUT_NON_CONSTEXPR std::basic_istream<Elem, Traits>& operator>>(
				std::basic_istream<Elem, Traits>& lhs,
				uniform_01&
				)
			{
				return lhs;
			}
			template<typename Elem, typename Traits>
			friend SPROUT_NON_CONSTEXPR std::basic_ostream<Elem, Traits>& operator<<(
				std::basic_ostream<Elem, Traits>& lhs,
				uniform_01 const&
				)
			{
				return lhs;
			}
			friend SPROUT_CONSTEXPR bool operator==(uniform_01 const&, uniform_01 const&) SPROUT_NOEXCEPT {
				return true;
			}
			friend SPROUT_CONSTEXPR bool operator!=(uniform_01 const& lhs, uniform_01 const& rhs) SPROUT_NOEXCEPT {
				return !(lhs == rhs);
			}
		};
	}	// namespace random

	using sprout::random::uniform_01;
}	// namespace sprout

#endif	// #ifndef SPROUT_RANDOM_UNIFORM_01_HPP
