#pragma once
#ifndef OTOS_TRAITS_SOLVER_H
#define OTOS_TRAITS_SOLVER_H

#include <cfp/traits/fwd/solver.h>

namespace cfp::traits {

  template<typename Problem>
  struct solver {
    using type = typename Problem::solver_type;
  };
}

#endif // OTOS_TRAITS_SOLVER_H
