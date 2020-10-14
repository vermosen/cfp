
#include "ewm.h"

// [[Rcpp::export]]
Rcpp::NumericVector ewm_cov(
    const Rcpp::NumericVector& ts_x
  , const Rcpp::NumericVector& ts_y
  , double alpha, bool bias, bool adjust) {

  if (ts_x.size() != ts_y.size())
    throw std::logic_error("time series size must match");

  auto rc = Rcpp::NumericVector(ts_x.size(), NA_REAL);

  if (rc.size() == 1)
    return rc;

  double old_wt_factor = 1.0 - alpha;
  auto new_wt = (adjust ? 1.0 : alpha);
  double mean_x = ts_x[0]; double mean_y = ts_y[0];
  bool valid = ((mean_x == mean_x) && (mean_y == mean_y));
  int nobs = static_cast<int>(valid);

  if (!valid) {
    mean_x = std::numeric_limits<double>::quiet_NaN();
    mean_y = std::numeric_limits<double>::quiet_NaN();
  }

  double cov = 0.0;
  double sum_wt = 1.0;
  double sum_wt2 = 1.0;
  double old_wt = 1.0;

  for (decltype(ts_x.size()) i = 1; i < ts_x.size(); i++) {
    auto x = ts_x[i];
    auto y = ts_y[i];

    valid = ((x == x) && (y == y));
    nobs += int(valid);

    if (mean_x == mean_x) {
      if (valid) {
        sum_wt *= old_wt_factor;
        sum_wt2 *= (old_wt_factor * old_wt_factor);
        old_wt *= old_wt_factor;

        auto old_mean_x = mean_x;
        auto old_mean_y = mean_y;

        if (mean_x != x) {
          mean_x = ((old_wt * old_mean_x) +
            (new_wt * x)) / (old_wt + new_wt);
        }

        if (mean_y != y) {
          mean_y = ((old_wt * old_mean_y) +
            (new_wt * y)) / (old_wt + new_wt);
        }

        cov = ((old_wt * (cov + ((old_mean_x - mean_x) *
          (old_mean_y - mean_y)))) +
          (new_wt * ((x - mean_x) * (y - mean_y)))) / (old_wt + new_wt);

        sum_wt += new_wt;
        sum_wt2 += (new_wt * new_wt);
        old_wt += new_wt;

        if (!adjust) {
          sum_wt /= old_wt;
          sum_wt2 /= (old_wt * old_wt);
          old_wt = 1.0;
        }
      }
    }
    else if (valid) {
      mean_x = x;
      mean_y = y;
    }

    if (!bias) {
      auto numerator = sum_wt * sum_wt;
      auto denominator = numerator - sum_wt2;

      if (denominator > std::numeric_limits<double>::epsilon()) {
        rc[i] = ((numerator / denominator) * cov);
      }
      else {
        rc[i] = std::numeric_limits<double>::quiet_NaN();
      }
    }
    else {
      rc[i] = cov;
    }
  }

  return rc;
}

