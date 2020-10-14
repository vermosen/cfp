#include "rolling.h"

#include <cfp/math/rolling/covariance.h>
#include <cfp/math/rolling/variance.h>

using namespace cfp;

// TODO: we actually don't need circular buffers here...

// [[Rcpp::export]]
Rcpp::NumericVector rolling_cov(
    const Rcpp::NumericVector& col1
  , const Rcpp::NumericVector& col2
  , std::size_t obs
  , std::size_t period
  , double fill) {

  if (period < 1)
    throw std::logic_error("zero-lenght period not valid");

  if (col1.size() != col2.size())
    throw std::logic_error("time series size must match");

  auto rc = Rcpp::NumericVector(col1.size(), fill);
  auto sz = static_cast<std::size_t>(rc.size());

  if (sz == 1)
    return rc;

  // TODO
  throw std::logic_error("not implemented!");

  //rolling<tags::covariance> roll(obs);

  //for (std::size_t pos = 0; pos < sz;) {

  //  // push a new value
  //  roll.add(col1[pos], col2[pos]);

  //  for (std::size_t sub = pos; sub < std::min(pos + period, sz); sub++) {
  //    //rc[sub] = roll.value();                           // fill/advance
  //  }

  //  pos += period;
  //}

  return rc;
}

// [[Rcpp::export]]
Rcpp::NumericVector rolling_var(
    const Rcpp::NumericVector& col
  , std::size_t obs
  , std::size_t period
  , double fill) {

  auto rc = Rcpp::NumericVector(col.size(), fill);
  auto sz = static_cast<std::size_t>(rc.size());

  if (period < 1)
    throw std::logic_error("zero-lenght period not valid");

  if (sz == 1)
    return rc;

  cfp::rolling<cfp::tags::variance> roll(obs);

  for (std::size_t pos = 0; pos < sz;) {

    // push a new value
    roll.add(col[pos]);

    for (std::size_t sub = pos; sub < std::min(pos + period, sz); sub++) {
      if (roll.valid()) {
        rc[sub] = roll.value();                             // fill/advance
      }
    }

    pos += period;
  }

  return rc;
}
