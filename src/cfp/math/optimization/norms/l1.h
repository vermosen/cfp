#pragma once
#ifndef CFP_MATH_OPTIMIZATION_NORMS_L1_H
#define CFP_MATH_OPTIMIZATION_NORMS_L1_H

#include <cfp/math/optimization/norm.h>

namespace cfp {

  template <>
  class norm<norms::type::l1> {
  public:
    template <typename Data>
    double apply(Data d) {
      return d.template lpNorm<1>();
    }
  };
}

#endif // CFP_MATH_OPTIMIZATION_NORMS_L1_H