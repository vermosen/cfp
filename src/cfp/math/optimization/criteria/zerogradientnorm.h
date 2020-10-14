#pragma once
#ifndef OTOS_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H
#define OTOS_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H

#include <cfp/math/optimization/criterion.h>

namespace cfp {

  template <typename Problem>
  class criterion<Problem, criteria::type::zeroGradientNorm> {
  public:
    using param_type = typename Problem::param_type;
  
  public:
    criterion(double tol) 
      : m_tol(tol) {}

    bool apply(const param_type& p) const {
      return (p.norm() < m_tol);
    }

  private:
    double m_tol;
  };
}

#endif // OTOS_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H
