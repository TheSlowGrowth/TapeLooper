/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPOST_SOURCES_SOURCE_HPP
#define SPROUT_COMPOST_SOURCES_SOURCE_HPP

#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/array/array.hpp>

//
// COMPOST_LOAD_SOURCE
//
#define COMPOST_LOAD_SOURCE <sprout/compost/load/source.hpp>

//
// COMPOST_SRC_VERSION
//
#define COMPOST_SRC_VERSION(NUM) NUM

namespace sprout {
	namespace compost {
		namespace sources {
			//
			// version_type
			//
			typedef unsigned long version_type;
			//
			// info_type
			//
			struct info_type {
			public:
				std::uint16_t format_tag;		// format ID
				std::uint16_t channels;			// channels
				std::uint32_t samples_per_sec;	// sampling rate
				std::uint32_t bytes_per_sec;	// data speed (Byte/sec)
				std::uint16_t block_size;		// block size (Byte/sample*channels)
				std::uint16_t bits_per_sample;	// bits per sample (bit/sample)
				std::size_t size;				// elements
			};
			//
			// sound_type
			//
			template<std::size_t Size, typename Elem = double>
			struct sound_type {
			public:
				typedef Elem element_type;
				typedef element_type value_type;
				typedef std::size_t size_type;
				SPROUT_STATIC_CONSTEXPR size_type static_size = Size;
				typedef sprout::array<value_type, static_size> elements_type;
			private:
				elements_type elements_;
			public:
				template<typename... Elems>
				SPROUT_CONSTEXPR sound_type(info_type const& info, Elems const&... elems)
					: elements_{{
						(info.bits_per_sample == 8 ? elems / static_cast<value_type>(0x80) - 1
							: elems / static_cast<value_type>(0x8000)
							)...
						}}
				{
					static_assert(sizeof...(Elems) == static_size, "sound_type<>: unmatch source size");
				}
				SPROUT_CONSTEXPR value_type const&
				operator()(size_type x) const {
					return elements_[x];
				}
				SPROUT_CONSTEXPR size_type
				size() const {
					return static_size;
				}
				SPROUT_CONSTEXPR elements_type const&
				elements() const {
					return elements_;
				}
			};
			template<std::size_t Size, typename Elem>
			SPROUT_CONSTEXPR_OR_CONST typename sprout::compost::sources::sound_type<Size, Elem>::size_type
			sprout::compost::sources::sound_type<Size, Elem>::static_size;
		}	// namespace sources
	}	// namespace compost
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPOST_SOURCES_SOURCE_HPP
