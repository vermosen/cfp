#pragma once
#ifndef OTOS_MODEL_SIMULATION_H
#define OTOS_MODEL_SIMULATION_H

#include <random>

namespace cfp {

  template <
      typename Problem
    , typename Device = std::mt19937
  > class simulation;
}

#endif // OTOS_MODEL_SIMULATION_H