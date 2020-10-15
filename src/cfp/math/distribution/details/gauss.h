#pragma once

#include <random>

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Eigen::internal {

  template<
      typename Scalar
    , typename Rng
  > class scalar_normal_dist_op {
  public:
    scalar_normal_dist_op(Rng& device) 
      : m_device(device) {}
    
    Rng& device() { return m_device; }

    template<typename Index>
    inline const Scalar operator() (Index, Index=0) const { return norm(m_device); }

  private:
    mutable std::normal_distribution<Scalar> norm;
    Rng& m_device;
  };

  template<typename Scalar, typename Rng>
  struct functor_traits<scalar_normal_dist_op<Scalar, Rng> > {
    enum {
        Cost = 50 * NumTraits<Scalar>::MulCost
      , PacketAccess = false
      , IsRepeatable = false
    };
  };
} // end namespace Eigen::internal
