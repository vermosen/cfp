#pragma once
#ifndef CFP_MATH_OPTIMIZATION_NORM_H
#define CFP_MATH_OPTIMIZATION_NORM_H

namespace cfp {

namespace norms {
  enum class type {
      l1     = 1
    , l2     = 2
    , linfty = 3
    , none   = 0
  };
}

  template <norms::type Type>
  class norm;
}

#endif // CFP_MATH_OPTIMIZATION_NORM_H