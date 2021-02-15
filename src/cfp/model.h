#pragma once
#ifndef CFP_MODEL_CFP_H
#define CFP_MODEL_CFP_H

#include <optional>

#include<Eigen/Core>
#include<Eigen/Dense>

#include <cfp/math/optimization/solvers/base.h>
#include <cfp/math/optimization/criteria/zerogradientnorm.h>

#include <cfp/model/cache.h>
#include <cfp/model/recorder.h>
#include <cfp/model/caches/cfp.h>
#include <cfp/model/parameters/cfp.h>

#include <cfp/traits/model.h>

#include <cfp/model/solvers/cfp/simple.h>
#include <cfp/model/recorders/cfp.h>
#include <cfp/model/recorders/none.h>

namespace cfp {

  template <
      typename T
    , int Size>
  class model {
  public:
    using matrix_type = Eigen::Matrix<T, Size, Size>;
    using vrow_type   = Eigen::Matrix<T, 1, Size>;
    using vcol_type   = Eigen::Matrix<T, Size, 1>;
    using data_type   = Eigen::Matrix<T, Eigen::Dynamic, 1>;
    using solver_type = solver<model<T, Size>, solvers::type::simple>;
    using cache_type  = cache<model<T, Size>, caches::type::simple>;

  public:
    using repr_type = Eigen::Matrix<T, Eigen::Dynamic, 1>;

  private:
    static constexpr double an11() { return 1.0; }
    static constexpr double qn22() { return 0.0; }

  public:
    model(const parameter<model<T, Size>>&);

    model(const model&);
    model& operator=(const model&);

  public:
    parameter<model<T, Size>> parameters() const; // public accessor

  //private:
    model(
        const matrix_type&
      , const matrix_type&
      , const matrix_type&
      , const matrix_type&
      , const data_type&
      , const vcol_type&
      , const matrix_type&
      , double
    ); // TODO instead: model(repr_type)

  public:

    template<recorders::type Type = recorders::type::none>
    void /* typename recorder<model<T, Size>, Type>::data_type  */
    filter(Eigen::Ref<const data_type> in, Eigen::Ref<data_type> out);
    
    void predict(Eigen::Ref<const data_type> in, std::size_t steps, Eigen::Ref<data_type> out);
    void smoother(Eigen::Ref<const data_type> in, Eigen::Ref<data_type> out);
    model::data_type simulate(std::size_t len, int seed) const; // put in base class
      
    template<recorders::type Type = recorders::type::none>
    typename recorder<model<T, Size>, Type>::data_type 
    emax(Eigen::Ref<const data_type> in
      , parameter<model<T, Size>>& out
      , std::size_t nstep, double tol);

  private:
    void filter_impl(
        Eigen::Ref<const data_type> in
      , Eigen::Ref<data_type> out
      , std::optional<cache_type>& c); // TODO: remove use of optional

    void predict_impl(
        Eigen::Ref<const data_type> in
      , int steps
      , Eigen::Ref<data_type> out
      , cache_type& c);

    void smoother_impl(
        Eigen::Ref<const data_type> in
      , Eigen::Ref<data_type> out
      , cache_type& cache);

    model<T, Size> emax_impl(
        Eigen::Ref<const data_type> in
      , cache_type& cache);

    // solver interface. TODO: push in base class
  private:
    template <typename, solvers::type> friend class solver;
    model(const repr_type&);
    repr_type representation() const;

  private:
    const vrow_type m_c = vrow_type::Constant(1.0);
    std::array<matrix_type, Size> m_A;
    std::array<matrix_type, Size> m_Q;
    data_type m_psi;
    vcol_type m_pi;
    matrix_type m_sigma;
    double m_r;
  };

  template <typename T, int Size>
  inline model<T, Size>::model(const repr_type& params) {

    m_A[0]  << params[0], 0.0, 0.0, params[1];
    m_A[1]  << an11(), 0.0, 0.0, params[1];
    m_Q[0]  << params[2], 0.0, 0.0, params[3];
    m_Q[1]  << qn22(), 0.0, 0.0, params[3];
    m_pi    << params[4], params[5];
    m_sigma << params[6], 0.0, 0.0, params[7];
    m_r = params[8];

    m_psi = Eigen::VectorXd::Zero(params.size() - 9);
    for (int i = 0; i < params.size() - 9; i++)
      m_psi[i] = params[i + 9];
  }

  template <typename T, int Size>
  inline model<T, Size>::model(const parameter<model<T, Size>>& params) {
    m_A[0]  << params.m_a_eta, 0.0, 0.0, params.m_a_mu;
    m_A[1]  << an11(), 0.0, 0.0, params.m_a_mu;
    m_Q[0]  << params.m_s_eta, 0.0, 0.0, params.m_s_mu;
    m_Q[1]  << qn22(), 0.0, 0.0, params.m_s_mu;
    m_pi    << params.m_pi_1, params.m_pi_2;
    m_sigma << params.m_sig_1, 0.0, 0.0, params.m_sig_2;
    m_r = params.m_r;

    m_psi = Eigen::VectorXd::Map(params.m_psi.data(), params.m_psi.size());
  }

  template <typename T, int Size>
  inline model<T, Size>::model(const model<T, Size>& o) {
    if (this != &o) {
      m_A     = o.m_A;
      m_Q     = o.m_Q;
      m_psi   = o.m_psi;
      m_pi    = o.m_pi;
      m_sigma = o.m_sigma;
      m_r     = o.m_r;
    }
  }

  template <typename T, int Size>
  inline model<T, Size>&
    model<T, Size>::operator=(const model& o) {
    if (this != &o) {
      m_A     = o.m_A;
      m_Q     = o.m_Q;
      m_psi   = o.m_psi;
      m_pi    = o.m_pi;
      m_sigma = o.m_sigma;
      m_r     = o.m_r;
    }

    return *this;
  }

  template <typename T, int Size>
  inline model<T, Size>::model(
      const matrix_type& Ap
    , const matrix_type& An
    , const matrix_type& Qp
    , const matrix_type& Qn
    , const data_type& psi
    , const vcol_type& pi
    , const matrix_type& sigma
    , double r)
    : m_A { Ap, An }
    , m_Q { Qp, Qn }
    , m_psi(psi), m_pi(pi)
    , m_sigma(sigma), m_r(r) {}

  template<typename T, int Size>
  inline parameter<model<T, Size>> 
  model<T, Size>::parameters() const {

    parameter<model<T, Size>> p;

    p.m_a_eta =  m_A[0](0, 0);
    p.m_a_mu  =  m_A[0](1, 1);
    p.m_s_eta =  m_Q[0](0, 0);
    p.m_s_mu  =  m_Q[0](1, 1);
    p.m_pi_1  =    m_pi(0, 0);
    p.m_pi_2  =    m_pi(1, 0);
    p.m_sig_1 = m_sigma(0, 0);
    p.m_sig_2 = m_sigma(1, 1);
    p.m_r     =     m_r      ;

    p.m_psi = std::vector<double>(m_psi.data()
      , m_psi.data() + m_psi.rows() * m_psi.cols());

    return p;
  }

  template <typename T, int Size>
  inline typename model<T, Size>::repr_type
  model<T, Size>::representation() const {

    repr_type p = repr_type::Zero(9 + m_psi.size());

    p <<
        m_A[0](0, 0)
      , m_A[0](1, 1)
      , m_Q[0](0, 0)
      , m_Q[0](1, 1)
      , m_pi[0]
      , m_pi[1]
      , m_sigma(0, 0)
      , m_sigma(1, 1)
      , m_r
      , m_psi
      ;

    return p;
  }

  template <typename T, int Size>
  template <recorders::type Type>
  inline void /* typename recorder<model<T, Size>, Type>::data_type */
  model<T, Size>::filter(
      Eigen::Ref<const data_type> in
    , Eigen::Ref<data_type> out) {

    std::optional<cache_type> c = std::nullopt;

    filter_impl(in, out, c);

    if constexpr (!(Type == recorders::type::none)) {
      // TODO
    } else {

    }
  }

  template <typename T, int Size>
  inline void model<T, Size>::predict(
      Eigen::Ref<const data_type> in
    , std::size_t steps
    , Eigen::Ref<data_type> out) {

    std::optional<cache_type> c(in.size());
    filter_impl(in, out, c);
    predict_impl(in, static_cast<int>(steps), out, *c);
  }

  template <typename T, int Size>
  /*template <recorders::type Recorder>*/
  inline void /*typename recorder<model<T, Size>::data_type*/ 
  model<T, Size>::smoother(
      Eigen::Ref<const data_type> in
    , Eigen::Ref<data_type> out) {

    std::optional<cache_type> c(in.size());
    filter_impl(in, out, c);
    smoother_impl(in, out, *c);
  }

  template <typename T, int Size>
  template <recorders::type Recorder>
  inline typename recorder<model<T, Size>, Recorder>::data_type
    model<T, Size>::emax(
        Eigen::Ref<const data_type> in
      , parameter<model<T, Size>>& out
      , std::size_t maxstep
      , double tol) {

    using criteria_type = criterion<
      model<T, Size>
      , criteria::type::zeroGradientNorm>;

    auto tmp = representation();
    Eigen::VectorXd dummy = Eigen::VectorXd::Zero(in.size());
    Eigen::Map<Eigen::VectorXd> mdummy(dummy.data(), dummy.size());
    model<T, Size> t = *this;

    solver_type s(in);
    criteria_type cr(tol);

    auto diagnostics = s.template minimize<Recorder, criteria_type>(tmp, cr, maxstep);
    
    // convert back to array
    out.m_a_eta = tmp[0];
    out.m_a_mu  = tmp[1];
    out.m_s_eta = tmp[2];
    out.m_s_mu  = tmp[3];
    out.m_pi_1  = tmp[4];
    out.m_pi_2  = tmp[5];
    out.m_sig_1 = tmp[6];
    out.m_sig_2 = tmp[7];
    out.m_r     = tmp[8];

    out.m_psi.clear(); // in case

    for (std::size_t i = 0; i < static_cast<std::size_t>(tmp.size() - 9); i++)
      out.m_psi.push_back(tmp[9 + i]);

    return diagnostics;
  }

  template<typename T, int Size>
  inline void model<T, Size>::filter_impl(
      Eigen::Ref<const data_type> in
    , Eigen::Ref<data_type> out
    , std::optional<cache_type>& c) {

    int period = m_psi.size();

    // initialization
    vcol_type pm, cm, gain;
    matrix_type pc, cc, lt;

    // initial values for cond mean, cond cov and y
    cm = m_pi; cc = m_sigma;
    out[0] = m_c * m_pi + m_psi[0];

    for (int i = 0; i < out.size(); i++) {

      if (c) {
        c->m_cmeans.push_back(cm);
        c->m_ccovs.push_back(cc);
      }

      int pos = (int)(i % period != 0);
      const matrix_type& amat = m_A[pos];
      const matrix_type& qmat = m_Q[pos];

      // kf calculations
      pm = amat * cm;
      pc = amat * cc * amat.transpose() + qmat;
      gain = (pc * m_c.transpose()) / (m_c * pc * m_c.transpose() + m_r);

      if (c) {
        c->m_pmeans.push_back(pm);
        c->m_pcovs.push_back(pc);
        c->m_gains.push_back(gain);
      }

      if (i < out.size() - 1) {                                                     // not on the last op
        auto val = in[i + 1];
        cm = pm + gain * (val - m_psi[(i + 1) % period] - m_c * pm);
        cc = pc - gain * m_c * pc;
        out[i + 1] = m_c * pm + m_psi[(i + 1) % period];                            // prediction
      }
    }

    if (c) {                                                                        // t + 1 entries used for the filter
      c->m_cmeans.push_back(cm);
      c->m_ccovs.push_back(cc);
    }

    return;
  }

  template <typename T, int Size>
  inline void model<T, Size>::predict_impl(
      Eigen::Ref<const data_type> in
    , int steps
    , Eigen::Ref<data_type> out
    , cache_type& c) {

    int t = in.size();
    int period = m_psi.size();
    out = data_type::Constant(t, NAN);          // we reset the output vector

    // initialization
    for (int i = 0; i < out.size() - steps; i++) {

      auto& x = c.m_cmeans[i];                  // the filtered xs produced by the kf step
      int ndays = (i + steps) / period;         // determine how many time we have to apply A[0]
      ndays -= (i / period);
      double psi = m_psi[(i + steps) % period];
      out[i]  = std::pow(m_A[0](0, 0), ndays) * std::pow(m_A[1](0, 0), steps - ndays) * x[0];
      out[i] += std::pow(m_A[0](1, 1), ndays) * std::pow(m_A[1](1, 1), steps - ndays) * x[1];
      out[i] += psi;
    }

    return;
  }

  template<typename T, int Size>
  inline void model<T, Size>::smoother_impl(
      Eigen::Ref<const data_type> in
    , Eigen::Ref<data_type> out
    , cache_type& c) {

    std::size_t t = in.size();
    int period = m_psi.size();

    vcol_type smean = c.m_cmeans[t - 1];                             // initial values for smean, scov
    matrix_type scov = c.m_ccovs[t - 1];
    matrix_type ccov = c.m_ccovs[t - 1];
    vcol_type cmean = c.m_cmeans[t - 1];
    matrix_type pcov = c.m_pcovs[t - 1];
    vcol_type pmean = c.m_pmeans[t - 1];

    out[t - 1] = m_c * smean + m_psi[(t - 1) % period];

    auto pos = (int)((t) % period != 0);                            // position should be i + 1 -> t !!!!
    matrix_type lt = ccov * m_A[pos].transpose() * pcov.inverse();
    c.m_lt[t - 1] = lt;                                             // compute l_n
    c.m_smeans[t - 1] = c.m_cmeans[t - 1];
    c.m_scovs[t - 1] = c.m_ccovs[t - 1];

    for (int i = t - 2; i >= 0; i--) {

      ccov = c.m_ccovs[i];
      cmean = c.m_cmeans[i];
      pcov = c.m_pcovs[i];
      pmean = c.m_pmeans[i];

      auto pos = (int)((i) % period != 0);                          // same as above
      lt = ccov * m_A[pos].transpose() * pcov.inverse();
      smean = cmean + lt * (smean - pmean);
      scov = ccov + lt * (scov - pcov) * lt.transpose();
      out[i] = m_c * smean + m_psi[i % period];

      c.m_lt[i] = lt;
      c.m_smeans[i] = smean;
      c.m_scovs[i] = scov;
    }
  }

  template<typename T, int Size>
  inline model<T, Size> model<T, Size>::emax_impl(
    Eigen::Ref<const data_type> in
    , cache_type& c) {

    matrix_type p, pcor, ccor;

    auto t = c.size();
    int period = m_psi.size();

    // last entry
    int pos = (int)((t - 2) % period != 0);
    ccor = (matrix_type::Identity() - c.m_gains[t - 2] * m_c) * m_A[pos] * c.m_ccovs[t - 2];   // equation A.21
    vcol_type x = c.m_smeans[t - 1];
    p = c.m_ccovs[t - 1] + x * x.transpose();
    pcor = ccor + c.m_smeans[t - 1] * c.m_smeans[t - 2].transpose();

    c.m_ps[t - 1] = p;
    c.m_pcors[t - 1] = pcor;
    c.m_xs[t - 1] = x;

    // we need to cache everything first
    for (std::size_t i = c.size() - 2; i > 0; --i) {

      pos = static_cast<int>(((i) % period != 0));

      x = c.m_smeans[i];
      p = c.m_ccovs[i] + x * x.transpose();
      ccor = c.m_ccovs[i] * c.m_lt[i - 1].transpose()
        + c.m_lt[i] * (ccor - m_A[pos] * c.m_ccovs[i]) * c.m_lt[i - 1].transpose();
      pcor = ccor + x * c.m_smeans[i - 1].transpose();

      c.m_xs[i] = x;
      c.m_ps[i] = p;
      c.m_pcors[i] = pcor;
      //c.m_ccors[i] = ccor;
    }

    // we can also compute these
    c.m_xs[0] = c.m_smeans[0];
    c.m_ps[0] = c.m_scovs[0] + c.m_smeans[0] * c.m_smeans[0].transpose();

    // pi_1 and sigma_1
    vcol_type pi_up = c.m_smeans[0];
    matrix_type sig_up = c.m_ps[0] - pi_up * pi_up.transpose();

    double a_eta_up, a_eta_down, a_mu_up, a_mu_down;

    // a_eta and a_mu
    a_eta_up = 0; a_eta_down = 0; a_mu_up = 0; a_mu_down = 0;
#ifdef DEBUG
    double a_neta_up, a_neta_down; a_neta_up = 0; a_neta_down = 0;
#endif
    for (std::size_t i = 1; i < c.size(); i++) {

      if ((i - 1) % period == 0) {
        auto num = c.m_pcors[i](0, 0);
        auto denom = c.m_ps[i - 1](0, 0);
        a_eta_up += num;
        a_eta_down += denom;
      }
#ifdef DEBUG
      else {
        a_neta_up += c.m_pcors[i](0, 0);
        a_neta_down += c.m_ps[i - 1](0, 0);
      }
#endif
      a_mu_up += c.m_pcors[i](1, 1);
      a_mu_down += c.m_ps[i - 1](1, 1);
    }

    a_eta_up /= a_eta_down;
    a_mu_up /= a_mu_down;

#ifdef DEBUG
    a_neta_up /= a_neta_down; // sanity check, should be close to 1...
#endif

    matrix_type ap_up; ap_up << a_eta_up, 0.0, 0.0, a_mu_up;
    matrix_type an_up; an_up << an11(), 0.0, 0.0, a_mu_up;

    // sigma
    double sig_eta = 0; double sig_mu = 0; int ctr = 0;

    for (std::size_t i = 1; i < c.size(); i++) {

      if (i % period == 0) {
        auto contrib = c.m_ps[i](0, 0)
          + std::pow(ap_up(0, 0), 2.0) * c.m_ps[i - 1](0, 0)
          - 2.0 * ap_up(0, 0) * c.m_pcors[i](0, 0);

        sig_eta += contrib;
        ctr++;
      }

      auto contrib = c.m_ps[i](1, 1)
        + std::pow(an_up(1, 1), 2.0) * c.m_ps[i - 1](1, 1)
        - 2.0 * an_up(1, 1) * c.m_pcors[i](1, 1);

      sig_mu += contrib;
    }

    sig_eta /= (ctr - 1);
    sig_mu /= (c.size() - 1);

    matrix_type qp_up, qn_up;
    const double eps = std::numeric_limits<double>::epsilon();
    qp_up << std::max(sig_eta, eps), 0.0, 0.0, std::max(sig_mu, eps);
    qn_up << qn22(), 0.0, 0.0, std::max(sig_mu, eps);

    // update psi
    data_type psi_up = data_type::Zero(m_psi.size());
    Eigen::VectorXi ctrs = Eigen::VectorXi::Zero(m_psi.size());

    for (std::size_t i = 0; i < c.size(); i++) {
      auto pos = i % period;
      psi_up[pos] += (in[i] - m_c * c.m_xs[i]);
      ctrs[pos] += 1;
    }

    for (std::size_t i = 0; i < psi_up.size(); i++) {   // can be replace by elementwise ops
      psi_up[i] /= ctrs[i];
    }

    // update r
    double r_up = 0.0;

    for (std::size_t i = 0; i < c.size(); i++) {
      auto psi = psi_up[i % period];
      r_up += std::pow(in[i], 2.0) + m_c * c.m_ps[i] * m_c.transpose()
        - 2.0 * in[i] * m_c * c.m_xs[i] + std::pow(psi, 2.0)
        - 2.0 * in[i] * psi + 2.0 * psi * m_c * c.m_xs[i];
    }

    r_up /= c.size();

    model<T, Size> model(
        ap_up
      , an_up
      , qp_up
      , qn_up
      , psi_up
      , pi_up
      , sig_up
      , r_up
    );

    return model;
  }

  template<typename T, int Size>
  inline typename model<T, Size>::data_type model<T, Size>::simulate(std::size_t len, int seed) const {

    auto params = parameters();
    simulation<model<T, Size>> sim(params, len, seed);
    return sim.next();
  }
}

#endif
