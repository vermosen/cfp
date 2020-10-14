
#include <limits>
#include <Rcpp.h>

// TODO: import from wotan package
std::vector<std::int64_t> to_int64(const Rcpp::NumericVector& v) {

  std::vector<double> in = Rcpp::as<std::vector<double>>(v);
  std::vector<std::int64_t> rv =
    *reinterpret_cast<std::vector<std::int64_t>*>(&in);
  return std::move(rv);
}

Rcpp::NumericVector from_int64(std::vector<std::int64_t>&& in) {
  const std::vector<double>& vec =
    *reinterpret_cast<const std::vector<double>*>(&in);
  Rcpp::NumericVector rv = Rcpp::wrap(vec);
  return std::move(rv);
}

Rcpp::NumericVector otos_ewma_period(
    const Rcpp::NumericVector& t
  , const Rcpp::NumericVector& x
  , double halflife) {

  auto tm = to_int64(t);

  if (x.size() == 0 || static_cast<long>(tm.size()) != x.size()) {
    throw std::logic_error("inconsistent data set");
  }

  Rcpp::NumericVector rv(tm.size(), NA_REAL);
  long first = 0;

  while (Rcpp::NumericVector::is_na(x[first]) && first < tm.size() - 1) {
    first++;
  }

  double val = x[first];
  rv[first] = val;

  if (first == tm.size() - 1) {
    return rv;
  }

  for (long i = first + 1; i < tm.size(); i++) {

    if (Rcpp::NumericVector::is_na(x[i])) {          // in case there are NAs along the way...
      rv[i] = rv[i - 1];
      continue;
    }
    else {
      double ewma, dt, alpha;
      dt = tm[i] - tm[i - 1];
      alpha = 1 - std::exp(std::log(0.5) * dt / halflife);
      ewma = alpha * x[i] + (1 - alpha) * rv[i - 1];
      rv[i] = ewma;
    }
  }

  return rv;
}


RCPP_MODULE(signals_module) {

  Rcpp::function("otos_ewma_period", &otos_ewma_period);
}