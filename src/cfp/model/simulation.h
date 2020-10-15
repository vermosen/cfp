#pragma once
#ifndef CFP_MODEL_SIMULATION_H
#define CFP_MODEL_SIMULATION_H

#include <random>

namespace cfp {

  /*!
   * @class simulation
   * @brief a simulation class
   * @tparam Problem the problem type
   * @tparam Device the random generator device
   */
  template <
      typename Problem
    , typename Device = std::mt19937
  > class simulation;
}

#endif // CFP_MODEL_SIMULATION_H