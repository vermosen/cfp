
#include <RcppUtils/eigen.h>  // utility classes for Rcpp 

#include<cfp/model/recorders/cfp.h>
#include<cfp/model/recorders/none.h>
#include<cfp/model/solvers/cfp/simple.h>
#include <cfp/model/simulations/cfp.h>

#include<cfp/model.h>

using param_type = cfp::parameter<cfp::model<double, 2>>;

RCPP_EXPOSED_CLASS_NODECL(param_type)

Rcpp::NumericVector simulate_cfp(
    const cfp::parameter<cfp::model<double, 2>>& p
  , std::size_t n, int seed) {

  auto model = cfp::model<double, 2>(p);
  Eigen::VectorXd out = model.simulate(n, seed);
  return Rcpp::wrap(out);
}

Rcpp::NumericVector filter_cfp(
    const param_type& params
  , const Rcpp::NumericVector& col) {

  std::size_t n = col.size();
  const auto in = Rcpp::as<std::vector<double>>(col);
  std::vector<double> out(n, NAN);

  cfp::model<double, 2> model(params);

  model.filter(
      Eigen::Map<const Eigen::VectorXd>(in.data(), in.size())
    , Eigen::Map<Eigen::VectorXd>(out.data(), out.size())
    );

  return Rcpp::wrap(out);
}

Rcpp::NumericVector smoother_cfp(
    const param_type& params
  , const Rcpp::NumericVector& col) {

  std::size_t n = col.size();
  const auto in = Rcpp::as<std::vector<double>>(col);
  std::vector<double> out(n, NAN);

  cfp::model<double, 2> model(params);

  model.smoother(
      Eigen::Map<const Eigen::VectorXd>(in.data(), in.size())
    , Eigen::Map<Eigen::VectorXd>(out.data(), out.size())
    );

  return Rcpp::wrap(out);
}

Rcpp::NumericVector predict_cfp(
    const param_type& params
  , const Rcpp::NumericVector& col
  , const std::size_t nsteps) {

  std::size_t n = col.size();

  if (nsteps > n)
    throw std::logic_error("nsteps have to be less than the data length");

  const auto in = Rcpp::as<std::vector<double>>(col);
  std::vector<double> out(n, NAN);

  cfp::model<double, 2> model(params);

  model.predict(
      Eigen::Map<const Eigen::VectorXd>(in.data(), in.size())
    , nsteps, Eigen::Map<Eigen::VectorXd>(out.data(), out.size())
    );

  return Rcpp::wrap(out);
}

cfp::parameter<cfp::model<double, 2>> em_cfp(
    const param_type& params
  , const Rcpp::NumericVector& col
  , std::size_t nstep
  , double tol) {

  std::size_t nrows = col.size();
  const auto in = Rcpp::as<std::vector<double>>(col);
  
  cfp::model<double, 2> model(params);
  cfp::parameter<cfp::model<double, 2>> out;

  model.emax<cfp::recorders::type::none>(
    Eigen::Map<const Eigen::VectorXd>(in.data(), in.size())
    , out, nstep, tol);

  return out;
}

Rcpp::DataFrame em_cfp_debug(
    const param_type& params
  , const Rcpp::NumericVector& col
  , std::size_t nstep
  , double tol) {

  std::size_t nrows = col.size();
  const auto in = Rcpp::as<std::vector<double>>(col);
  cfp::parameter<cfp::model<double, 2>> out;
  cfp::model<double, 2> model(params);

  auto diagnostics = model.emax<cfp::recorders::type::simple>(
    Eigen::Map<const Eigen::VectorXd>(in.data(), in.size())
    , out, nstep, tol);

  auto sz = diagnostics.size();

  auto rstep  = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto ra_eta = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto ra_mu  = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rs_eta = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rs_mu  = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rpi_1  = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rpi_2  = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rsig_1 = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rsig_2 = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);
  auto rr     = Rcpp::NumericVector(diagnostics[0].size(), NA_REAL);

  std::vector<Rcpp::NumericVector> rpsi;

  for (std::size_t j = 9; j < diagnostics.size(); j++) {
    rpsi.push_back(Rcpp::NumericVector(diagnostics[j].size(), NA_REAL));
  }

  for (std::size_t i = 0; i < diagnostics[0].size(); i++) {
    rstep[i]  = i;
    ra_eta[i] = diagnostics[0][i];
    ra_mu[i]  = diagnostics[1][i];
    rs_eta[i] = diagnostics[2][i];
    rs_mu[i]  = diagnostics[3][i];
    rpi_1[i]  = diagnostics[4][i];
    rpi_2[i]  = diagnostics[5][i];
    rsig_1[i] = diagnostics[6][i];
    rsig_2[i] = diagnostics[7][i];
    rr[i]     = diagnostics[8][i];

    for (std::size_t j = 9; j < diagnostics.size(); j++) {
      rpsi[j-9][i] = diagnostics[j][i];
    }
  }

  Rcpp::DataFrame result = Rcpp::DataFrame::create(
      Rcpp::_("step")  = rstep
    , Rcpp::_("a.eta") = ra_eta
    , Rcpp::_("a.mu")  = ra_mu
    , Rcpp::_("s.eta") = rs_eta
    , Rcpp::_("s.mu")  = rs_mu
    , Rcpp::_("pi.1")  = rpi_1
    , Rcpp::_("pi.2")  = rpi_2
    , Rcpp::_("sig.1") = rsig_1
    , Rcpp::_("sig.2") = rsig_2
    , Rcpp::_("r")     = rr
    );

  for (std::size_t j = 0; j < diagnostics.size() - 9; j++) {
    std::stringstream ss; ss << "psi." << j;
    result.push_back(rpsi[j], ss.str());
  }

  return result;
}

RCPP_MODULE(cfp_module) {

  using param_type = cfp::parameter<cfp::model<double, 2>>;

  Rcpp::class_<param_type>("cfp_parameters")
    .constructor()
    .field("a.eta", &param_type::m_a_eta)
    .field("a.mu" , &param_type::m_a_mu )
    .field("s.eta", &param_type::m_s_eta)
    .field("s.mu" , &param_type::m_s_mu )
    .field("pi.1" , &param_type::m_pi_1 )
    .field("pi.2" , &param_type::m_pi_2 )
    .field("sig.1", &param_type::m_sig_1)
    .field("sig.2", &param_type::m_sig_2)
    .field("psi"  , &param_type::m_psi  )
    .field("r"    , &param_type::m_r    )
    ;

  Rcpp::function("simulate_cfp", &simulate_cfp );
  Rcpp::function("filter_cfp"  , &filter_cfp   );
  Rcpp::function("smoother_cfp", &smoother_cfp );
  Rcpp::function("predict_cfp" , &predict_cfp  );
  Rcpp::function("em_cfp"      , &em_cfp       );
  Rcpp::function("em_cfp_debug", &em_cfp_debug );
}