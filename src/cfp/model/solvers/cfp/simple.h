#pragma once
#ifndef CFP_MODEL_SOLVERS_CFP_SIMPLE_H
#define CFP_MODEL_SOLVERS_CFP_SIMPLE_H

#include <optional>

#include<Eigen/Core>
#include<Eigen/Dense>

#include <cfp/model/recorder.h>
#include <cfp/model.h>

#include <cfp/math/optimization/solver.h>

namespace cfp {

  // simple solver 
  template <
    typename T
    , int Size
  >
  class solver<model<T, Size>, solvers::type::simple>
    : public solvers::base<solver<model<T, Size>, solvers::type::simple>> {
    private:
      using ancestor = solvers::base<solver<model<T, Size>, solvers::type::simple>>;
    public:
      using model_type = model<T, Size>;
      using data_type  = typename model_type::data_type;
      using param_type = typename ancestor::param_type;

    public:
      solver(Eigen::Ref<const data_type> data);

      template<recorders::type Recorder, typename Criteria>
      typename recorder<model<T, Size>, Recorder>::data_type
      minimize(Eigen::Ref<param_type>, Criteria, std::size_t);
  };

  template <typename T, int Size>
  inline solver<model<T, Size>, solvers::type::simple>::solver(
    Eigen::Ref<const typename model_type::data_type> data)
    : ancestor(data) {}

  template <typename T, int Size>
  template <recorders::type Recorder, typename Criteria>
  typename recorder<model<T, Size>, Recorder>::data_type
  inline solver<model<T, Size>, solvers::type::simple>::minimize(
        Eigen::Ref<param_type> start
      , Criteria cr
      , std::size_t maxsteps) {

    auto& data = ancestor::m_data;

    model<T, Size> it(start);
    Eigen::VectorXd dummy = Eigen::VectorXd::Zero(data.size());             // dummy array
    recorder<model<T, Size>, Recorder> rec(start.size(), maxsteps);

    bool filled = false;

    do {
      std::optional<typename model<T, Size>::cache_type> c(data.size());   // whipe the cache on every iteration
      it.filter_impl(data, Eigen::Ref<data_type>(dummy), c);
      it.smoother_impl(data, Eigen::Ref<data_type>(dummy), *c);
      model<T, Size> res = it.emax_impl(data, *c);
      auto params = res.representation();
      filled = cr.template apply(it.representation() - params);
      rec.record(params);
      it = res;

    } while (--maxsteps > 0 && !filled);

    start = it.representation();
    return rec.data();
  }
}

#endif
