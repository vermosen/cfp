#pragma once
#ifndef CFP_FWD_RCPP_UTILS_H
#define CFP_FWD_RCPP_UTILS_H

#include <complex>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace Rcpp   {
namespace traits {

  template <typename, typename>
  class matrix_exporter;

  template<typename>
  class Exporter;

  template<typename T> class Exporter< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> >;
  template<typename T> class Exporter< Eigen::Matrix<T, Eigen::Dynamic, 1> >;
  template<typename T> class Exporter< Eigen::Matrix<T, 1, Eigen::Dynamic> >;
  template<typename T> class Exporter< Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> >;
  template<typename T> class Exporter< Eigen::Array<T, Eigen::Dynamic, 1> >;
  template<typename T> class Exporter< Eigen::Array<T, 1, Eigen::Dynamic> >;
  template<typename T> class Exporter< Eigen::SparseMatrix<T> >;
  template<typename T> class Exporter< Eigen::SparseMatrix<T, Eigen::RowMajor> >;

  template<typename T> class Exporter< Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::Matrix<T, 1, Eigen::Dynamic> > >;

  // TODO: Map<const Eigen::Matrix>

  // further spec required for std::complex types
  template<typename T> class Exporter< Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, 1> > >;

  template<typename T> class Exporter< Eigen::Map<Eigen::Array<T, Eigen::Dynamic, 1> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::SparseMatrix<T> > >;
  template<typename T> class Exporter< Eigen::Map<Eigen::SparseMatrix<T, Eigen::RowMajor> > >;
}}

#endif // CFP_FWD_RCPP-UTILS_H