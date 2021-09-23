/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_STRING_TYPE_TRAITS_HPP
#define SPROUT_STRING_TYPE_TRAITS_HPP

#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/string/string.hpp>
#include <sprout/type_traits/integral_constant.hpp>

namespace sprout {
	//
	// string_t
	//
	template<std::size_t N>
	struct string_t {
	public:
		typedef sprout::basic_string<char, N> type;
	};
	//
	// wstring_t
	//
	template<std::size_t N>
	struct wstring_t {
	public:
		typedef sprout::basic_string<wchar_t, N> type;
	};
#if SPROUT_USE_UNICODE_LITERALS
	//
	// u16string_t
	//
	template<std::size_t N>
	struct u16string_t {
	public:
		typedef sprout::basic_string<char16_t, N> type;
	};
	//
	// u32string_t
	//
	template<std::size_t N>
	struct u32string_t {
	public:
		typedef sprout::basic_string<char32_t, N> type;
	};
#endif

	//
	// is_basic_string
	//
	template<typename T>
	struct is_basic_string
		: public sprout::false_type
	{};
	template<typename T>
	struct is_basic_string<T const>
		: public sprout::is_basic_string<T>
	{};
	template<typename T>
	struct is_basic_string<T const volatile>
		: public sprout::is_basic_string<T>
	{};
	template<typename T, std::size_t N, typename Traits>
	struct is_basic_string<sprout::basic_string<T, N, Traits> >
		: public sprout::true_type
	{};

	//
	// is_string_of
	//
	template<typename T, typename Elem>
	struct is_string_of
		: public sprout::false_type
	{};
	template<typename T, typename Elem>
	struct is_string_of<T const, Elem>
		: public sprout::is_string_of<T, Elem>
	{};
	template<typename T, typename Elem>
	struct is_string_of<T const volatile, Elem>
		: public sprout::is_string_of<T, Elem>
	{};
	template<typename T, std::size_t N, typename Traits, typename Elem>
	struct is_string_of<sprout::basic_string<T, N, Traits>, Elem>
		: public sprout::true_type
	{};

	//
	// is_string
	//
	template<typename T>
	struct is_string
		: public sprout::is_string_of<T, char>
	{};
	//
	// is_wstring
	//
	template<typename T>
	struct is_wstring
		: public sprout::is_string_of<T, wchar_t>
	{};
#if SPROUT_USE_UNICODE_LITERALS
	//
	// is_u16string
	//
	template<typename T>
	struct is_u16string
		: public sprout::is_string_of<T, char16_t>
	{};
	//
	// is_u32string
	//
	template<typename T>
	struct is_u32string
		: public sprout::is_string_of<T, char32_t>
	{};
#endif

#if SPROUT_USE_VARIABLE_TEMPLATES
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_basic_string_v = sprout::is_basic_string<T>::value;
	template<typename T, typename Elem>
	SPROUT_STATIC_CONSTEXPR bool is_string_of_v = sprout::is_string_of<T, Elem>::value;
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_string_v = sprout::is_string<T>::value;
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_wstring_v = sprout::is_wstring<T>::value;
#if SPROUT_USE_UNICODE_LITERALS
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_u16string_v = sprout::is_u16string<T>::value;
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_u32string_v = sprout::is_u32string<T>::value;
#endif
#endif	// #if SPROUT_USE_VARIABLE_TEMPLATES
}	// namespace sprout

#endif	// #ifndef SPROUT_STRING_TYPE_TRAITS_HPP
