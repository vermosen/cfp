#pragma once
#ifndef OTOS_TRAITS_MODEL_H
#define OTOS_TRAITS_MODEL_H

#include <cfp/traits/fwd/model.h>
#include <cfp/math/optimization/solver.h>
#include <cfp/model/simulation.h>

namespace cfp::traits {

  template<typename Model, solvers::type Type>
  struct model<solver<Model, Type>> {
    using type = Model;
  };

  template<typename Model, typename Device>
  struct model<cfp::simulation<Model, Device>> {
    using type = Model;
  };
}

#endif // OTOS_TRAITS_MODEL_H
