/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RATIONAL_RATIONAL_HPP
#define SPROUT_RATIONAL_RATIONAL_HPP

#include <utility>
#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/rational/exceptions.hpp>
#include <sprout/math/gcd.hpp>
#include <sprout/utility/swap.hpp>
#include <sprout/detail/call_traits.hpp>

namespace sprout {
	template<typename IntType>
	class rational;

	namespace detail {
		struct rational_private_construct_t {};

		template<typename IntType>
		class rational_construct_access;
	}	// namespace detail

	namespace detail {
		template<typename IntType>
		class rational_impl {
		protected:
			typedef IntType int_type;
			typedef typename sprout::detail::call_traits<IntType>::param_type param_type;
		protected:
			IntType num_;
			IntType den_;
		protected:
			SPROUT_CONSTEXPR rational_impl()
				: num_(0), den_(1)
			{}
			rational_impl(rational_impl const&) = default;
			SPROUT_CONSTEXPR rational_impl(param_type n)
				: num_(n), den_(1)
			{}
			SPROUT_CONSTEXPR rational_impl(param_type n, param_type d)
				: num_(n), den_(d)
			{}
			SPROUT_CONSTEXPR rational_impl(param_type n, param_type d, param_type g)
				: num_(n / g), den_(d / g)
			{}
		};
	}	// namespace detail

	//
	// rational
	//
	template<typename IntType>
	class rational
		: private sprout::detail::rational_impl<IntType>
	{
		static_assert(sprout::numeric_limits<IntType>::is_specialized, "sprout::numeric_limits<IntType>::is_specialized");
		friend class sprout::detail::rational_construct_access<IntType>;
	public:
		typedef IntType int_type;
		typedef typename sprout::detail::call_traits<IntType>::param_type param_type;
	private:
		typedef sprout::detail::rational_impl<IntType> base_type;
	private:
		static SPROUT_CONSTEXPR IntType normalize_g_1(IntType den, IntType g) {
			return den / g < 0 ? -g : g;
		}
		static SPROUT_CONSTEXPR IntType normalize_g(IntType num, IntType den) {
			return den == 0 ? throw sprout::bad_rational()
				: num == 0 ? den
				: normalize_g_1(den, sprout::gcd(num, den))
				;
		}
	private:
		using base_type::num_;
		using base_type::den_;
	private:
		SPROUT_CONSTEXPR rational(sprout::detail::rational_private_construct_t, param_type n, param_type d)
			: base_type(n, d)
		{}
	public:
		SPROUT_CONSTEXPR rational() SPROUT_NOEXCEPT
			: base_type()
		{}
		rational(rational const&) = default;
		SPROUT_CONSTEXPR rational(param_type n) SPROUT_NOEXCEPT
			: base_type(n)
		{}
		SPROUT_CONSTEXPR rational(param_type n, param_type d)
			: base_type(n, d, normalize_g(n, d))
		{}

		SPROUT_CXX14_CONSTEXPR rational& operator=(rational const& rhs) SPROUT_NOEXCEPT {
			rational temp(rhs);
			sprout::swap(temp, *this);
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator=(param_type n) SPROUT_NOEXCEPT {
			return assign(n, 1);
		}
		SPROUT_CXX14_CONSTEXPR rational& assign(param_type n, param_type d) {
			rational temp(n, d);
			sprout::swap(temp, *this);
			return *this;
		}

		SPROUT_CONSTEXPR IntType numerator() const SPROUT_NOEXCEPT {
			return num_;
		}
		SPROUT_CONSTEXPR IntType denominator() const SPROUT_NOEXCEPT {
			return den_;
		}

		SPROUT_CXX14_CONSTEXPR rational& operator+=(rational const& rhs) {
			IntType g = sprout::gcd(den_, rhs.den_);
			den_ /= g;
			num_ = num_ * (rhs.den_ / g) + rhs.num_ * den_;
			g = sprout::gcd(num_, g);
			num_ /= g;
			den_ *= rhs.den_ / g;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator-=(rational const& rhs) {
			IntType g = sprout::gcd(den_, rhs.den_);
			den_ /= g;
			num_ = num_ * (rhs.den_ / g) - rhs.num_ * den_;
			g = sprout::gcd(num_, g);
			num_ /= g;
			den_ *= rhs.den_ / g;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator*=(rational const& rhs) {
			IntType gcd1 = sprout::gcd(num_, rhs.den_);
			IntType gcd2 = sprout::gcd(rhs.num_, den_);
			num_ =(num_ / gcd1) * (rhs.num_ / gcd2);
			den_ =(den_ / gcd2) * (rhs.den_ / gcd1);
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator/=(rational const& rhs) {
			if (rhs.num_ == IntType(0)) {
				throw sprout::bad_rational();
			}
			if (num_ == IntType(0)) {
				return *this;
			}
			IntType gcd1 = sprout::gcd(num_, rhs.num_);
			IntType gcd2 = sprout::gcd(rhs.den_, den_);
			num_ =(num_ / gcd1) * (rhs.den_ / gcd2);
			den_ =(den_ / gcd2) * (rhs.num_ / gcd1);
			if (den_ < IntType(0)) {
				num_ = -num_;
				den_ = -den_;
			}
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator+=(param_type rhs) {
			return *this += rational(rhs);
		}
		SPROUT_CXX14_CONSTEXPR rational& operator-=(param_type rhs) {
			return *this -= rational(rhs);
		}
		SPROUT_CXX14_CONSTEXPR rational& operator*=(param_type rhs) {
			return *this *= rational(rhs);
		}
		SPROUT_CXX14_CONSTEXPR rational& operator/=(param_type rhs) {
			return *this /= rational(rhs);
		}

		SPROUT_CXX14_CONSTEXPR rational& operator++() SPROUT_NOEXCEPT {
			num_ += den_;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational& operator--() SPROUT_NOEXCEPT {
			num_ -= den_;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR rational operator++(int) SPROUT_NOEXCEPT {
			rational result(*this);
			++*this;
			return result;
		}
		SPROUT_CXX14_CONSTEXPR rational operator--(int) SPROUT_NOEXCEPT {
			rational result(*this);
			--*this;
			return result;
		}

		SPROUT_CONSTEXPR bool operator!() const SPROUT_NOEXCEPT {
			return !num_;
		}
		SPROUT_CONSTEXPR operator bool() const SPROUT_NOEXCEPT {
			return num_ != 0;
		}
	};

	namespace detail {
		template<typename IntType>
		class rational_construct_access {
		public:
			static SPROUT_CONSTEXPR sprout::rational<IntType>
			raw_construct(
				typename sprout::detail::call_traits<IntType>::param_type n,
				typename sprout::detail::call_traits<IntType>::param_type d
				)
			{
				return sprout::rational<IntType>(
					sprout::detail::rational_private_construct_t(),
					n, d
					);
			}
		};
	}	// namespace detail
}	// namespace sprout

#endif	// SPROUT_RATIONAL_RATIONAL_HPP
