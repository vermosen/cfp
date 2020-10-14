#pragma once
#ifndef OTOS_TRAITS_PROBLEM_H
#define OTOS_TRAITS_PROBLEM_H

#include <cfp/traits/fwd/problem.h>

namespace cfp::traits {

  template<typename Solver>
  struct problem {
    using type = typename Solver::problem_type;
  };
}

#endif
