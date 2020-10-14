#include "debug.h"

#include <exception>

//' Throw an exception from Rcpp
//'
//' @export
//' @examples
//'
//' library(cfp)
//'
//' throw_exc()
//'
// [[Rcpp::export]]
double throw_exc() {
  throw(Rcpp::exception("My Error Message"));
  return 0.0;
}
 