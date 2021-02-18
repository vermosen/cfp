#pragma once
#ifndef CFP_TRAITS_CACHE_H
#define CFP_TRAITS_CACHE_H

#include <optional> // nullopt_t
#include <cfp/traits/fwd/cache.h>
#include <cfp/model/cache.h>

namespace cfp::traits {

	template <typename Model, caches::type C>
	struct cache {
		using type = ::cfp::cache<Model, C>;
	};

  template <typename Model>
	struct cache<Model, caches::type::none> {
		using type = std::nullopt_t;
	};
}

#endif // CFP_TRAITS_DEVICE_H