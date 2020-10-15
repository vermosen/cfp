#pragma once
#ifndef CFP_MATH_SOLVER_H
#define CFP_MATH_SOLVER_H

namespace cfp {

namespace solvers {
  enum class type {
      simple = 1
    , none   = 0
  };
}

  template <
      typename Model
    , solvers::type Type
  >
  class solver;
}

#endif
