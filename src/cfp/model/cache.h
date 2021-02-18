#pragma once
#ifndef CFP_MODEL_CACHE_H
#define CFP_MODEL_CACHE_H

namespace cfp    {
namespace caches {

  enum class type {
      simple = 1
    , none   = 0
  };
}

  template<typename Model, caches::type>
  struct cache;
}

#endif // CFP_MODEL_CACHE_H
