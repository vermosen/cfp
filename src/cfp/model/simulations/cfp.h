#pragma once
#ifndef CFP_MODEL_SIMULATION_CFP_H
#define CFP_MODEL_SIMULATION_CFP_H

#include <cfp/model.h>

#include <cfp/traits/parameter.h>
#include <cfp/traits/model.h>

#include <cfp/model/parameters/cfp.h>
#include <cfp/model/simulations/base.h>

#include <cfp/math/distribution/gaussian_mult.h>

namespace cfp {

  template <typename Device>
  class simulation<model<double, 2>, Device> 
    : public simulations::base<simulation<model<double, 2>, Device>> {
  private:
    using parent = simulations::base<simulation<model<double, 2>, Device>>;

  public:
    template <std::size_t Sz>
    using sampler_t = gaussian_mult<double, Sz>;

  public:
    simulation(
        const parameter<model<double, 2>>& params
      , std::size_t len
      , int seed);
    
    friend parent;
  private:
    Eigen::VectorXd next_impl(); 
  };

  template <typename Device>
  inline simulation<model<double, 2>, Device>::simulation(
      const parameter<model<double, 2>>& params
    , std::size_t len, int seed)
    : parent(params, len, seed) {}

  template <typename Device>
  inline Eigen::VectorXd 
  simulation<model<double, 2>, Device>::next_impl() {
  
    auto& device = parent::m_device;
    auto& p = parent::m_params;
    auto& sz = parent::m_size;

    double eps = std::numeric_limits<double>::epsilon();
    Eigen::VectorXd result = Eigen::VectorXd::Zero(sz);
    Eigen::Matrix2d A_n, A_p, Q_n, Q_p;
    A_p << p.m_a_eta, 0.0, 0.0, p.m_a_mu;
    A_n << 1.0      , 0.0, 0.0, p.m_a_mu;
    Q_p << p.m_s_eta, 0.0, 0.0, p.m_s_mu;
    Q_n << eps      , 0.0, 0.0, p.m_s_mu;

    Eigen::Vector2d   x; x   << p.m_pi_1, p.m_pi_2;
    Eigen::Matrix2d sig; sig << p.m_sig_1, 0.0, 0.0, p.m_sig_2;
    
    Eigen::Ref<const Eigen::VectorXd> psi = 
      Eigen::Map<const Eigen::VectorXd>(
        p.m_psi.data(), p.m_psi.size());
    std::size_t period = psi.size();

    auto sampler_n = sampler_t<2>(Eigen::Vector2d::Zero(), Q_n, device);
    auto sampler_p = sampler_t<2>(Eigen::Vector2d::Zero(), Q_p, device);
    auto sampler_r = sampler_t<1>(0, p.m_r, device);

    //Eigen::Vector2d gauss = sampler_n.sample(); // matrix of multivariate gaussian samples
 
    for (std::size_t i = 0; i < parent::m_size; i++) {
      if (i % period == 0) {
        //result[i] =
      } else {
      
      }
    }

    return Eigen::VectorXd::Zero(10);
  }
}

#endif // CFP_MODEL_SIMULATION_CFP_H