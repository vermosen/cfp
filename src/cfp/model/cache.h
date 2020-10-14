#pragma once
#ifndef OTOS_MODEL_CACHE_H
#define OTOS_MODEL_CACHE_H

namespace cfp    {
namespace caches  {

  enum class type {
      simple = 1
    , none   = 0
  };
}

  template<typename Model, caches::type>
  struct cache;
}

#endif // OTOS_MODEL_CACHE_H
