#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

#include <cfp/model/simulation.h>
#include <cfp/traits/fwd/device.h>
#include <cfp/traits/fwd/parameter.h>
#include <cfp/traits/fwd/model.h>

namespace cfp::simulations {

  template <typename Child>
  class base {
  public:
    using model_type     = typename traits::model<Child>::type; 
    using parameter_type = typename traits::parameter<Child>::type;
    using device_type    = typename traits::device<Child>::type;

  public:
    base(const parameter_type&, std::size_t, int);
    Eigen::VectorXd next();
    void reset(int);

  protected:
    parameter_type m_params;
    device_type m_device;
    std::size_t m_size;
  };

  template <typename Child>
  inline base<Child>::base(const parameter_type& params, std::size_t sz, int seed)
    : m_params(params)
    , m_device(seed)
    , m_size(sz)
    {}

  template <typename Child>
  inline Eigen::VectorXd base<Child>::next() {        // todo: set seed ?
    return static_cast<Child&>(*this).next_impl();
  }

  template <typename Child>
  inline void base<Child>::reset(int seed) {
    m_device.seed(seed);
  }
}