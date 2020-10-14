#include "eigen.h"

//' Convert a matrix to eigen
//'
//' @export
//' @examples
//'
//' library(cfp)
//'
//' mat <- matrix(c(1.0, 1.0, 1.0, 1.0), nrow=2)
//' res <- cfp::norm(mat)
//'
// [[Rcpp::export]]
double norm(const Eigen::Map<Eigen::MatrixXd>& mat) {
  return mat.norm();
}

//' compute the norm of a complex matrix
//'
//' @export
//' @examples
//'
//' library(cfp)
//'
//' mat <- matrix(c(1.0+1.0i, 1.0+1.0i, 1.0+1.0i, 1.0+1.0i), nrow=2)
//' res <- cfp::cnorm(mat)
//'
// [[Rcpp::export]]
double cnorm(const Eigen::Map<Eigen::MatrixXcd>& mat) {
  return mat.norm();
}

//' compute the product C * adj(C)
//'
//' @export
//' @examples
//'
//' library(cfp)
//'
//' mat <- matrix(c(1.0+1.0i, 1.0+1.0i, 1.0+1.0i, 1.0+1.0i), nrow=2)
//' res <- cfp::selfadj_mult(mat)
//'
// [[Rcpp::export]]
Eigen::MatrixXd selfadj_mult(const Eigen::Map<Eigen::MatrixXcd>& mat) {
  Eigen::MatrixXd result = (mat * mat.adjoint()).real();
  return result;
}