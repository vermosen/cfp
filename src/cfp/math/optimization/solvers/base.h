#pragma once
#ifndef CFP_MATH_OPTIMIZATION_SOLVERS_BASE_H
#define CFP_MATH_OPTIMIZATION_SOLVERS_BASE_H

#include<Eigen/Core>
#include<Eigen/Dense>

#include <cfp/model/cache.h>
#include <cfp/model/recorder.h>
#include <cfp/traits/fwd/model.h>

namespace cfp    {
namespace solvers {

  template <
    typename Child
  >
  class base {
  public:
    using model_type = typename traits::model<Child>::type;
    using param_type = typename model_type::param_type;
    using data_type  = typename model_type::data_type ;

    public:
      base(Eigen::Ref<const data_type> data)
        : m_data(data) {}

      template<recorders::type Recorder, typename Criteria>
      typename recorder<model_type, Recorder>::data_type
        apply(Eigen::Ref<param_type> guess, Criteria cr, std::size_t maxit) {
        return static_cast<Child&>(*this).apply_impl(guess, cr, maxit);
      }

    protected:
      Eigen::Ref<const data_type> m_data;
  };
}}

#endif // CFP_MATH_OPTIMIZATION_SOLVERS_BASE_H
