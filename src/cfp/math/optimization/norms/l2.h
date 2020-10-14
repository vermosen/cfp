#pragma once
#ifndef OTOS_MATH_OPTIMIZATION_NORMS_L2_H
#define OTOS_MATH_OPTIMIZATION_NORMS_L2_H

#include <cfp/math/optimization/norm.h>

namespace cfp {

  template <>
  class norm<norms::type::l2> {
  public:
    template <typename Data>
    double apply(Data d) {
      return d.template lpNorm<2>();
    }
  };
}

#endif // OTOS_MATH_OPTIMIZATION_NORMS_L2_H