#pragma once
#ifndef OTOS_SRC_ROLLING_H
#define OTOS_SRC_ROLLING_H

#include <Rcpp.h>

Rcpp::NumericVector rolling_cov(
    const Rcpp::NumericVector& col1
  , const Rcpp::NumericVector& col2
  , std::size_t obs
  , std::size_t period
  , double fill);

Rcpp::NumericVector rolling_var(
    const Rcpp::NumericVector& col
  , std::size_t obs
  , std::size_t period
  , double fill);

#endif
