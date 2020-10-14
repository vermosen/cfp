
#include <rcpp-utils.h>  // from inst/include

#include <cfp/math/fft/plan.h>
#include <cfp/math/fft/welch.h>
#include <cfp/math/fft/convolve.h>
#include <cfp/math/fft/windows/all.h>
#include <cfp/meta/patterns/fwd/dictionary.h>

OTOS_DECLARE_ENUM(
  cfp::fft::flag,
  (measure        , "measure"        )
  (destroy_input  , "destroy_input"  )
  (unaligned      , "unaligned"      )
  (conserve_memory, "conserve_memory")
  (exhaustive     , "exhaustive"     )
  (preserve_input , "preserve_input" )
  (patient        , "patient"        )
  (estimate       , "estimate"       )
  (wisdom_only    , "wisdom_only"    )
)

#include <cfp/meta/patterns/dictionary.h>

namespace Rcpp {

  template <> cfp::fft::flag as(SEXP);
  template <> SEXP wrap(const cfp::fft::flag&);
}

#include <Rcpp.h>

namespace Rcpp {

  template <>
  cfp::fft::flag as(SEXP s) {
    auto f = ::Rcpp::as<std::string>(s);
    return cfp::util::dictionary<cfp::fft::flag>::value(f);
  }

  template <>
  SEXP wrap(const cfp::fft::flag& o) {
    auto f = cfp::util::dictionary<cfp::fft::flag>::key(o); 
    return ::Rcpp::wrap<std::string>(f);
  };
}

cfp::fft::flag make_flag(const std::string& v) {
  auto retval = cfp::util::dictionary<cfp::fft::flag>::value(v);
  return retval;
}

Eigen::VectorXd convolve_real(
    Eigen::Map<Eigen::VectorXd> lhs
  , Eigen::Map<Eigen::VectorXd> rhs
  , cfp::fft::plan& p) {
  return cfp::fft::convolve_real2(p, lhs, rhs);
}

Eigen::VectorXd convolve_real_debug(
    Eigen::Map<Eigen::VectorXd> lhs
  , Eigen::Map<Eigen::VectorXd> rhs) {
  return cfp::fft::convolve_real(lhs, rhs);
}

Eigen::VectorXd welch_real(
    Eigen::Map<Eigen::VectorXd> lhs
  , Eigen::Map<Eigen::VectorXd> rhs
  , cfp::fft::plan& p) {
  //return cfp::fft::welch(lhs, rhs, p);
  return Eigen::VectorXd::Zero(1);
}

RCPP_EXPOSED_CLASS_NODECL(cfp::fft::plan);
RCPP_EXPOSED_ENUM_NODECL(cfp::fft::flag);

RCPP_MODULE(fft_module) {

  Rcpp::function("otos_convolve_real", &convolve_real);
  Rcpp::function("otos_convolve_real_debug", &convolve_real_debug);
  Rcpp::function("otos_fft_flag", &make_flag);
  Rcpp::function("otos_fft_welch", &welch_real);

  Rcpp::class_<cfp::fft::plan>("otos_fft_plan")
    .constructor<Eigen::Map<Eigen::VectorXd>, cfp::fft::flag>()
    .property("flag" , &cfp::fft::plan::flags  )
    .method("execute", &cfp::fft::plan::execute)
    ;

  Rcpp::class_<cfp::fft::window>("otos_fft_window")
    ;

  Rcpp::class_<cfp::fft::windows::cosine>("otos_fft_window_cosine")
    .constructor<Eigen::Map<Eigen::VectorXd>>()
    .method("get", &cfp::fft::windows::cosine::get)
    ;
}
