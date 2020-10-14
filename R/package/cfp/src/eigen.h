#pragma once
#ifndef OTOS_SRC_EIGEN_H
#define OTOS_SRC_EIGEN_H

#include <Eigen/Core>
#include <Eigen/Dense>

#include <Rcpp.h>

double norm(const Eigen::Map<Eigen::MatrixXd>& mat);
double cnorm(const Eigen::Map<Eigen::MatrixXcd>& mat);
Eigen::MatrixXd selfadj_mult(const Eigen::Map<Eigen::MatrixXcd>& mat);

#endif
