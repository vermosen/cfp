#pragma once
#ifndef CFP_MATH_DISTRIBUTION_GAUSSIAN_MULT_H
#define CFP_MATH_DISTRIBUTION_GAUSSIAN_MULT_H

#include <cfp/math/distribution/details/gauss.h>

namespace cfp {

  template <
      typename Scalar
    , int Dim
    , typename Rng = std::mt19937
  >
  class gaussian_mult {
  
    using device_t = Eigen::internal::scalar_normal_dist_op<Scalar, Rng>;
  public:
    gaussian_mult(
        Eigen::Ref<const Eigen::Matrix<Scalar, Dim, 1>> mean
      , Eigen::Ref<const Eigen::Matrix<Scalar, Dim, Dim>> cov
      , Rng& device);

    Eigen::Matrix<Scalar, Dim, 1> 
    sample() const;

  private:
    Eigen::Matrix<Scalar, Dim, 1> m_mean;
    Eigen::Matrix<Scalar, Dim, Dim> m_L;
    device_t m_device;
  };  

  template <
      typename Scalar
    , int Dim
    , typename Rng
  >
  inline gaussian_mult<Scalar, Dim, Rng>::gaussian_mult(
      Eigen::Ref<const Eigen::Matrix<Scalar, Dim, 1>> mean
    , Eigen::Ref<const Eigen::Matrix<Scalar, Dim, Dim>> cov
    , Rng& rng)
    : m_mean(mean)
    , m_device(rng) {

    using matrix_t = Eigen::Matrix<Scalar, Dim, Dim>;

    if constexpr (Dim == Eigen::Dynamic) {

      auto sz = m_mean.rows();
      m_L = matrix_t::Zero(sz, sz);
      
      if (sz != cov.cols() || sz != cov.rows())
        throw std::logic_error(
          "mean and covariance dimensions are incompatible");
    }
    else {
      m_L = matrix_t::Zero();
    }

    Eigen::LLT<matrix_t> cholesky(cov);                           

    if (cholesky.info() == Eigen::Success) {
      m_L = cholesky.matrixL();                                 // Use cholesky solver
    }
    else {
      Eigen::SelfAdjointEigenSolver<matrix_t> solver(cov);      // If failed, use self adj solver
      m_L = solver.eigenvectors()
        * solver.eigenvalues().cwiseSqrt().asDiagonal();
    }
  }

  template <
      typename Scalar
    , int Dim
    , typename Rng
  >
  inline Eigen::Matrix<Scalar, Dim, 1> 
  gaussian_mult<Scalar, Dim, Rng>::sample() const {

    using vector_t = Eigen::Matrix<Scalar, Dim, 1>;
    std::size_t sz = m_mean.rows();
    return (m_L * vector_t::NullaryExpr(sz, 1, m_device))
      .colwise() + m_mean;
  }

  template <
      typename Scalar
    , typename Rng
  >
  class gaussian_mult<Scalar, 1, Rng> {
  private:
    using device_t = Eigen::internal::scalar_normal_dist_op<Scalar, Rng>;

  public:
    gaussian_mult(
        Scalar mean
      , Scalar cov
      , Rng& device) 
      : m_device(device)
      , m_mean(mean)
      , m_cov(cov) {}

    Scalar sample() const {
      return m_mean + std::sqrt(m_cov) * m_device(0);
    }

  private:
    device_t m_device;
    Scalar m_mean;
    Scalar m_cov;
  };
}

#endif // CFP_MATH_DISTRIBUTION_GAUSSIAN_MULT_H