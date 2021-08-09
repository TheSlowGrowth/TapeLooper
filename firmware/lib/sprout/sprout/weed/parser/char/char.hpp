/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_WEED_PARSER_CHAR_CHAR_HPP
#define SPROUT_WEED_PARSER_CHAR_CHAR_HPP

#include <iterator>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/string.hpp>
#include <sprout/container/begin.hpp>
#include <sprout/container/end.hpp>
#include <sprout/iterator/next.hpp>
#include <sprout/type_traits/identity.hpp>
#include <sprout/weed/unused.hpp>
#include <sprout/weed/parser_result.hpp>
#include <sprout/weed/parser/parser_base.hpp>
#include <sprout/weed/parser/lit.hpp>
#include <sprout/weed/traits/type/is_char_type.hpp>
#include <sprout/weed/traits/type/is_c_str.hpp>
#include <sprout/weed/traits/type/is_string.hpp>
#include <sprout/weed/detail/find_character_set.hpp>
#include <sprout/weed/detail/c_str_as_string.hpp>

namespace sprout {
	namespace weed {
		//
		// lit_char_p
		//
		template<typename T>
		struct lit_char_p
			: public sprout::weed::parser_base
		{
		public:
			template<typename Context, typename Iterator>
			struct attribute
				: public sprout::identity<sprout::weed::unused>
			{};
			template<typename Context, typename Iterator>
			struct result
				: public sprout::identity<sprout::weed::parser_result<Iterator, typename attribute<Context, Iterator>::type> >
			{};
		private:
			T t_;
		public:
			SPROUT_CONSTEXPR lit_char_p() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			explicit SPROUT_CONSTEXPR lit_char_p(T const& t)
				: t_(t)
			{}
			template<typename Context, typename Iterator>
			SPROUT_CONSTEXPR typename result<Context, Iterator>::type operator()(
				Iterator first, Iterator last,
				Context const&
				) const
			{
				typedef typename result<Context, Iterator>::type result_type;
				typedef typename attribute<Context, Iterator>::type attribute_type;
				typedef typename std::iterator_traits<Iterator>::value_type elem_type;
				return first != last && *first == elem_type(t_)
					? result_type(true, sprout::next(first), attribute_type())
					: result_type(false, first, typename attribute<Context, Iterator>::type())
					;
			}
		};
		//
		// lit_g
		//
		template<typename T>
		struct lit_g::eval<
			T,
			typename std::enable_if<
				sprout::weed::traits::is_char_type<T>::value
			>::type
		> {
		public:
			typedef sprout::weed::lit_char_p<T> result_type;
		public:
			SPROUT_CONSTEXPR result_type operator()(T const& t) const {
				return result_type(t);
			}
		};

		//
		// char_p
		//
		template<typename T>
		struct char_p
			: public sprout::weed::parser_base
		{
		public:
			template<typename Context, typename Iterator>
			struct attribute
				: public sprout::identity<typename std::iterator_traits<Iterator>::value_type>
			{};
			template<typename Context, typename Iterator>
			struct result
				: public sprout::identity<sprout::weed::parser_result<Iterator, typename attribute<Context, Iterator>::type> >
			{};
		private:
			T t_;
		public:
			SPROUT_CONSTEXPR char_p() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			explicit SPROUT_CONSTEXPR char_p(T const& t)
				: t_(t)
			{}
			template<typename Context, typename Iterator>
			SPROUT_CONSTEXPR typename result<Context, Iterator>::type operator()(
				Iterator first, Iterator last,
				Context const&
				) const
			{
				typedef typename result<Context, Iterator>::type result_type;
				typedef typename attribute<Context, Iterator>::type attribute_type;
				typedef typename std::iterator_traits<Iterator>::value_type elem_type;
				return first != last && *first == elem_type(t_)
					? result_type(true, sprout::next(first), *first)
					: result_type(false, first, attribute_type())
					;
			}
		};

		//
		// char_range_p
		//
		template<typename T>
		struct char_range_p
			: public sprout::weed::parser_base
		{
		public:
			template<typename Context, typename Iterator>
			struct attribute
				: public sprout::identity<typename std::iterator_traits<Iterator>::value_type>
			{};
			template<typename Context, typename Iterator>
			struct result
				: public sprout::identity<sprout::weed::parser_result<Iterator, typename attribute<Context, Iterator>::type> >
			{};
		private:
			T f_;
			T l_;
		public:
			SPROUT_CONSTEXPR char_range_p() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			SPROUT_CONSTEXPR char_range_p(T const& f, T const& l)
				: f_(f), l_(l)
			{}
			template<typename Context, typename Iterator>
			SPROUT_CONSTEXPR typename result<Context, Iterator>::type operator()(
				Iterator first, Iterator last,
				Context const&
				) const
			{
				typedef typename result<Context, Iterator>::type result_type;
				typedef typename attribute<Context, Iterator>::type attribute_type;
				typedef typename std::iterator_traits<Iterator>::value_type elem_type;
				return first != last && *first >= elem_type(f_) && *first <= elem_type(l_)
					? result_type(true, sprout::next(first), *first)
					: result_type(false, first, attribute_type())
					;
			}
		};

		//
		// char_set_p
		//
		template<typename T>
		struct char_set_p
			: public sprout::weed::parser_base
		{
		public:
			template<typename Context, typename Iterator>
			struct attribute
				: public sprout::identity<typename std::iterator_traits<Iterator>::value_type>
			{};
			template<typename Context, typename Iterator>
			struct result
				: public sprout::identity<sprout::weed::parser_result<Iterator, typename attribute<Context, Iterator>::type> >
			{};
		private:
			T t_;
		public:
			SPROUT_CONSTEXPR char_set_p() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			explicit SPROUT_CONSTEXPR char_set_p(T const& t)
				: t_(t)
			{}
			template<typename Context, typename Iterator>
			SPROUT_CONSTEXPR typename result<Context, Iterator>::type operator()(
				Iterator first, Iterator last,
				Context const&
				) const
			{
				typedef typename result<Context, Iterator>::type result_type;
				typedef typename attribute<Context, Iterator>::type attribute_type;
				return first != last && sprout::weed::detail::find_character_set(sprout::begin(t_), sprout::end(t_), *first) != sprout::end(t_)
					? result_type(true, sprout::next(first), *first)
					: result_type(false, first, attribute_type())
					;
			}
		};

		//
		// any_char_p
		//
		struct any_char_p
			: public sprout::weed::parser_base
		{
		public:
			template<typename Context, typename Iterator>
			struct attribute
				: public sprout::identity<typename std::iterator_traits<Iterator>::value_type>
			{};
			template<typename Context, typename Iterator>
			struct result
				: public sprout::identity<sprout::weed::parser_result<Iterator, typename attribute<Context, Iterator>::type> >
			{};
		public:
			template<typename Context, typename Iterator>
			SPROUT_CONSTEXPR typename result<Context, Iterator>::type operator()(
				Iterator first, Iterator last,
				Context const&
				) const
			{
				typedef typename result<Context, Iterator>::type result_type;
				typedef typename attribute<Context, Iterator>::type attribute_type;
				return first != last
					? result_type(true, sprout::next(first), *first)
					: result_type(false, first, attribute_type())
					;
			}
			template<typename T>
			SPROUT_CONSTEXPR typename std::enable_if<
				sprout::weed::traits::is_char_type<T>::value,
				sprout::weed::char_p<T>
			>::type
			operator()(T const& t) const {
				return sprout::weed::char_p<T>(t);
			}
			template<typename T>
			SPROUT_CONSTEXPR typename std::enable_if<
				sprout::weed::traits::is_char_type<T>::value,
				sprout::weed::char_range_p<T>
			>::type
			operator()(T const& f, T const& l) const {
				return sprout::weed::char_range_p<T>(f, l);
			}
			template<typename T>
			SPROUT_CONSTEXPR typename std::enable_if<
				sprout::weed::traits::is_c_str<T const>::value,
				sprout::weed::char_set_p<
					typename sprout::weed::detail::c_str_as_string<T const>::type
				>
			>::type
			operator()(T const& t) const {
				return sprout::weed::char_set_p<
					typename sprout::weed::detail::c_str_as_string<T const>::type
					>(sprout::to_string(t));
			}
			template<typename T>
			SPROUT_CONSTEXPR typename std::enable_if<
				sprout::weed::traits::is_string<T>::value,
				sprout::weed::char_set_p<T>
			>::type
			operator()(T const& t) const {
				return sprout::weed::char_set_p<T>(t);
			}
		};
		//
		// char_
		//
		SPROUT_STATIC_CONSTEXPR sprout::weed::any_char_p char_ = sprout::weed::any_char_p();
	}	// namespace weed
}	// namespace sprout

#endif	// #ifndef SPROUT_WEED_PARSER_CHAR_CHAR_HPP
