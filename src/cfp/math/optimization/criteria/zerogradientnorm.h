#pragma once
#ifndef CFP_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H
#define CFP_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H

#include <cfp/math/optimization/criterion.h>

namespace cfp {

  template <typename Problem>
  class criterion<Problem, criteria::type::zeroGradientNorm> {
  public:
    using param_type = typename Problem::repr_type;
  
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

#endif // CFP_MATH_OPTIMIZATION_CRITERIA_ZERO_GRADIENTNORM_H
