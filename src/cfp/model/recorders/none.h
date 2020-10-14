#pragma once
#ifndef OTOS_MODEL_RECORDERS_NONE_H
#define OTOS_MODEL_RECORDERS_NONE_H

#include<Eigen/Core>
#include<Eigen/Dense>

#include <cfp/model/recorder.h>

namespace cfp {

  template <typename Model>
  class recorder<Model, recorders::type::none> {
  public:
    using data_type = int; // can't return void here
    recorder(std::size_t, std::size_t) {}
    void record(const Eigen::VectorXd&) {}
    int data() const { return 0; }
  };
}

#endif // OTOS_MODEL_RECORDERS_NONE_H
