/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_UUID_NAME_GENERATOR_HPP
#define SPROUT_UUID_NAME_GENERATOR_HPP

#include <sprout/config.hpp>
#include <sprout/uuid/md5_name_generator.hpp>
#include <sprout/uuid/sha1_name_generator.hpp>

namespace sprout {
	namespace uuids {
		//
		// name_generator
		//
		typedef sprout::uuids::sha1_name_generator name_generator;
	}	// namespace uuids
}	// namespace sprout

#endif	// #ifndef SPROUT_UUID_NAME_GENERATOR_HPP
