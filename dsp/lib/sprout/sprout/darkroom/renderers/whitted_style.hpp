/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_DARKROOM_RENDERERS_WHITTED_STYLE_HPP
#define SPROUT_DARKROOM_RENDERERS_WHITTED_STYLE_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/limits.hpp>
#include <sprout/utility/pair.hpp>
#include <sprout/tuple/functions.hpp>
#include <sprout/tuple/tuple/type_traits.hpp>
#include <sprout/tuple/tuple/make_tuple.hpp>
#include <sprout/tuple/tuple/get.hpp>
#include <sprout/tuple/operation/push_back.hpp>
#include <sprout/tuple/operation/pop_back.hpp>
#include <sprout/darkroom/access/access.hpp>
#include <sprout/darkroom/colors/rgb.hpp>
#include <sprout/darkroom/coords/vector.hpp>
#include <sprout/darkroom/rays/ray.hpp>
#include <sprout/darkroom/materials/material.hpp>
#include <sprout/darkroom/intersects/intersection.hpp>
#include <sprout/darkroom/objects/intersect.hpp>
#include <sprout/darkroom/lights/calculate.hpp>
#include <sprout/darkroom/renderers/calculate.hpp>
#include <sprout/darkroom/renderers/infinity.hpp>

namespace sprout {
	namespace darkroom {
		namespace renderers {
			//
			// whitted_mirror
			//
			class whitted_mirror {
			private:
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection, typename Renderer,
					typename Direction
				>
				SPROUT_CONSTEXPR Color
				color_1(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, Intersection const& inter, Renderer const& renderer,
					std::size_t depth_max, Refractions const& refracs,
					Direction const& new_dir
					) const
				{
					return sprout::darkroom::renderers::calculate<Color>(
						renderer,
						camera, objs, lights,
						sprout::tuples::remake<Ray>(
							ray,
							sprout::darkroom::rays::detach_position(sprout::darkroom::intersects::point_of_intersection(inter), new_dir),
							new_dir
							),
						depth_max - 1,
						refracs
						);
				}
			public:
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection, typename Renderer
				>
				SPROUT_CONSTEXPR Color
				operator()(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, Intersection const& inter, Renderer const& renderer,
					std::size_t depth_max, Refractions const& refracs
					) const
				{
					typedef typename std::decay<
						decltype(sprout::darkroom::materials::reflection(sprout::darkroom::intersects::material(inter)))
					>::type reflection_type;
					return depth_max > 0
						&& sprout::darkroom::intersects::does_intersect(inter)
						&& sprout::darkroom::materials::reflection(sprout::darkroom::intersects::material(inter))
							> sprout::numeric_limits<reflection_type>::epsilon()
						? color_1<Color>(
							camera, objs, lights,
							ray, inter, renderer,
							depth_max, refracs,
							sprout::darkroom::coords::reflect(
								sprout::darkroom::rays::direction(ray),
								sprout::darkroom::intersects::normal(inter)
								)
							)
						: sprout::tuples::make<Color>(0, 0, 0)
						;
				}
			};
			//
			// whitted_transmit
			//
			class whitted_transmit {
			private:
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection, typename Renderer,
					typename Direction
				>
				SPROUT_CONSTEXPR sprout::pair<Color, bool>
				color_1(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, Intersection const& inter, Renderer const& renderer,
					std::size_t depth_max, Refractions const& refracs,
					Direction const& new_dir
					) const
				{
					return sprout::darkroom::coords::is_zero(new_dir) ? sprout::pair<Color, bool>(sprout::tuples::make<Color>(0, 0, 0), true)
						: sprout::darkroom::intersects::is_from_inside(inter) ? sprout::pair<Color, bool>(
							sprout::darkroom::renderers::calculate<Color>(
								renderer,
								camera, objs, lights,
								sprout::tuples::remake<Ray>(
									ray,
									sprout::darkroom::rays::detach_position(sprout::darkroom::intersects::point_of_intersection(inter), new_dir),
									new_dir
									),
								depth_max - 1,
								sprout::tuples::remake<Refractions>(refracs, 1.0)
								//sprout::tuples::pop_back(refracs)
								),
							false
							)
						: sprout::pair<Color, bool>(
							sprout::darkroom::renderers::calculate<Color>(
								renderer,
								camera, objs, lights,
								sprout::tuples::remake<Ray>(
									ray,
									sprout::darkroom::rays::detach_position(sprout::darkroom::intersects::point_of_intersection(inter), new_dir),
									new_dir
									),
								depth_max - 1,
								sprout::tuples::remake<Refractions>(refracs, sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter)))
								//sprout::tuples::push_back(refracs, sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter)))
								),
							false
							)
						;
				}
			public:
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection, typename Renderer
				>
				SPROUT_CONSTEXPR sprout::pair<Color, bool>
				operator()(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, Intersection const& inter, Renderer const& renderer,
					std::size_t depth_max, Refractions const& refracs
					) const
				{
					typedef typename std::decay<
						decltype(sprout::darkroom::materials::alpha(sprout::darkroom::intersects::material(inter)))
					>::type alpha_type;
					typedef typename std::decay<
						decltype(sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter)))
					>::type refraction_type;
					return depth_max > 0
						&& sprout::darkroom::intersects::does_intersect(inter)
						&& sprout::darkroom::materials::alpha(sprout::darkroom::intersects::material(inter))
							> sprout::numeric_limits<alpha_type>::epsilon()
						&& sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter))
							> sprout::numeric_limits<refraction_type>::epsilon()
						? color_1<Color>(
							camera, objs, lights,
							ray, inter, renderer,
							depth_max, refracs,
							sprout::darkroom::coords::refract(
								sprout::darkroom::rays::direction(ray),
								sprout::darkroom::intersects::normal(inter),
								sprout::darkroom::intersects::is_from_inside(inter)
									? sprout::tuples::get<sprout::tuples::tuple_size<Refractions>::value - 1>(refracs)
										/ sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter))
									: sprout::darkroom::materials::refraction(sprout::darkroom::intersects::material(inter))
										/ sprout::tuples::get<sprout::tuples::tuple_size<Refractions>::value - 1>(refracs)
								)
							)
						: sprout::pair<Color, bool>(sprout::tuples::make<Color>(0, 0, 0), false)
						;
				}
			};
			//
			// whitted_style
			//
			template<typename InfinityColor = sprout::darkroom::renderers::direction_gradation>
			class whitted_style {
			public:
				typedef InfinityColor infinity_color_type;
			private:
				infinity_color_type infinity_color_;
			private:
				template<typename Color, typename Ray, typename Intersection, typename DiffuseColor, typename MirrorColor, typename TransparentColors>
				SPROUT_CONSTEXPR Color
				color_3(
					Ray const& ray, Intersection const& inter,
					DiffuseColor const& diffuse_color, MirrorColor const& mirror_color, TransparentColors const& transmit_colors
					) const
				{
					return sprout::darkroom::intersects::does_intersect(inter)
						? sprout::darkroom::colors::add(
							sprout::darkroom::colors::mul(
								diffuse_color,
								1
									- sprout::darkroom::materials::reflection(sprout::darkroom::intersects::material(inter))
									- sprout::darkroom::materials::alpha(sprout::darkroom::intersects::material(inter))
								),
							sprout::darkroom::colors::mul(
								mirror_color,
								!transmit_colors.second
									? sprout::darkroom::materials::reflection(sprout::darkroom::intersects::material(inter))
									: sprout::darkroom::materials::reflection(sprout::darkroom::intersects::material(inter))
										+ sprout::darkroom::materials::alpha(sprout::darkroom::intersects::material(inter))
								),
							sprout::darkroom::colors::mul(
								transmit_colors.first,
								!transmit_colors.second
									? sprout::darkroom::materials::alpha(sprout::darkroom::intersects::material(inter))
									: 0
								)
							)
						: sprout::darkroom::renderers::calculate_infinity<Color>(infinity_color_, sprout::darkroom::rays::direction(ray))
						;
				}
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection
				>
				SPROUT_CONSTEXPR Color
				color_2(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, std::size_t depth_max, Refractions const& refracs, Intersection const& inter,
					Color const& diffuse_color
					) const
				{
					return color_3<Color>(
						ray, inter,
						diffuse_color,
						sprout::darkroom::renderers::whitted_mirror().template operator()<Color>(
							camera, objs, lights,
							ray, inter, *this,
							depth_max, refracs
							),
						sprout::darkroom::renderers::whitted_transmit().template operator()<Color>(
							camera, objs, lights,
							ray, inter, *this,
							depth_max, refracs
							)
						);
				}
				template<
					typename Color,
					typename Camera, typename Objects, typename Lights,
					typename Ray, typename Refractions, typename Intersection
				>
				SPROUT_CONSTEXPR Color
				color_1(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, std::size_t depth_max, Refractions const& refracs, Intersection const& inter
					) const
				{
					return color_2<Color>(
						camera, objs, lights,
						ray, depth_max, refracs, inter,
						sprout::darkroom::lights::calculate(lights, inter, objs)
						);
				}
			public:
				SPROUT_CONSTEXPR whitted_style()
					: infinity_color_()
				{}
				whitted_style(whitted_style const&) = default;
				explicit SPROUT_CONSTEXPR whitted_style(infinity_color_type const& infinity_color)
					: infinity_color_(infinity_color)
				{}
				template<
					typename Color, typename Camera, typename Objects, typename Lights, typename Ray, typename Refractions,
					typename sprout::enabler_if<sprout::tuples::is_tuple<Refractions>::value>::type = sprout::enabler
				>
				SPROUT_CONSTEXPR Color
				operator()(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, std::size_t depth_max, Refractions const& refracs
					) const
				{
					return color_1<Color>(
						camera, objs, lights,
						ray, depth_max, refracs,
						sprout::darkroom::objects::intersect(objs, ray)
						);
				}
				template<
					typename Color, typename Camera, typename Objects, typename Lights, typename Ray, typename Refractions,
					typename sprout::enabler_if<!sprout::tuples::is_tuple<Refractions>::value>::type = sprout::enabler
				>
				SPROUT_CONSTEXPR Color
				operator()(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, std::size_t depth_max, Refractions const& refracs
					) const
				{
					return operator()<Color>(
						camera, objs, lights,
						ray, depth_max, sprout::tuples::make_tuple(refracs)
						);
				}
				template<typename Color, typename Camera, typename Objects, typename Lights, typename Ray>
				SPROUT_CONSTEXPR Color
				operator()(
					Camera const& camera, Objects const& objs, Lights const& lights,
					Ray const& ray, std::size_t depth_max
					) const
				{
					return operator()<Color>(
						camera, objs, lights,
						ray, depth_max, sprout::tuples::make_tuple(1.0)
						);
				}
			};
			//
			// make_whitted_style
			//
			inline SPROUT_CONSTEXPR sprout::darkroom::renderers::whitted_style<>
			make_whitted_style() {
				return sprout::darkroom::renderers::whitted_style<>();
			}
			template<typename InfinityColor>
			inline SPROUT_CONSTEXPR sprout::darkroom::renderers::whitted_style<InfinityColor>
			make_whitted_style(InfinityColor const& infinity_color) {
				return sprout::darkroom::renderers::whitted_style<InfinityColor>(infinity_color);
			}
		}	// namespace renderers
	}	// namespace darkroom
}	// namespace sprout

#endif	// #ifndef SPROUT_DARKROOM_RENDERERS_WHITTED_STYLE_HPP
