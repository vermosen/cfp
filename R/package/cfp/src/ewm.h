#pragma once
#ifndef OTOS_SRC_EWMA_H
#define OTOS_SRC_EWMA_H

#include <Rcpp.h>

Rcpp::NumericVector ewm_cov(
    const Rcpp::NumericVector& ts_x
  , const Rcpp::NumericVector& ts_y
  , double alpha
  , bool bias
  , bool adjust);

#endif
