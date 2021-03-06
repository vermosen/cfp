#pragma once
#ifndef CFP_MODEL_CACHES_NONE_H
#define CFP_MODEL_CACHES_NONE_H

#include <cfp/model/cache.h>

namespace cfp    {

  // dummy cacbe
  template<typename Model, caches::type>
  struct cache <Model, caches::type::none> {
    cache() = delete;
    cache(std::size_t sz) {}

  private:
    std::size_t size() const { return 0; }
    void reset() {}
  };
}

#endif // CFP_MODEL_CACHES_NONE_H
