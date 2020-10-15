#pragma once
#ifndef CFP_TRAITS_SOLVER_H
#define CFP_TRAITS_SOLVER_H

#include <cfp/traits/fwd/solver.h>

namespace cfp::traits {

  template<typename Problem>
  struct solver {
    using type = typename Problem::solver_type;
  };
}

#endif // CFP_TRAITS_SOLVER_H
