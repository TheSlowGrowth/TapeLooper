/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_PREPROCESSOR_SOME_NUMBER_HPP
#define SPROUT_PREPROCESSOR_SOME_NUMBER_HPP

#include <sprout/config.hpp>
#include <sprout/preprocessor/config.hpp>
#include <sprout/preprocessor/empty.hpp>

//
// SPROUT_PP_SOME_NUMBER_IS_UNIQUE_IN_LINE
//
#if SPROUT_PP_HAS_COUNTER
#	undef SPROUT_PP_SOME_NUMBER_IS_UNIQUE_IN_LINE
#	define SPROUT_PP_SOME_NUMBER_IS_UNIQUE_IN_LINE 1
#else
#	undef SPROUT_PP_SOME_NUMBER_IS_UNIQUE_IN_LINE
#	define SPROUT_PP_SOME_NUMBER_IS_UNIQUE_IN_LINE 0
#endif

//
// SPROUT_PP_SOME_NUMBER
// SPROUT_PP_SOME_NUMBER_OR_EMPTY
//
#if SPROUT_PP_HAS_COUNTER
#	define SPROUT_PP_SOME_NUMBER() __COUNTER__
#	define SPROUT_PP_SOME_NUMBER_OR_EMPTY() SPROUT_PP_SOME_NUMBER()
#else
#	define SPROUT_PP_SOME_NUMBER() __LINE__
#	define SPROUT_PP_SOME_NUMBER_OR_EMPTY() SPROUT_PP_EMPTY()
#endif

#endif	// #ifndef SPROUT_PREPROCESSOR_SOME_NUMBER_HPP
