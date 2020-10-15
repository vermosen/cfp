#pragma once
#ifndef CFP_MODEL_PARAMETERS_CFP_H
#define CFP_MODEL_PARAMETERS_CFP_H

#include <vector>
#include <ostream>

#include <cfp/model/parameter.h>

#include <cfp/model.h>

#include <cfp/traits/device.h>
#include <cfp/traits/parameter.h>
#include <cfp/traits/model.h>

namespace cfp {

  template<>
  struct parameter<model<double, 2>> {
    parameter() {}
    double m_a_eta = 0.0;
    double m_a_mu = 0.0;
    double m_s_eta = 0.0;
    double m_s_mu = 0.0;
    double m_pi_1 = 0.0;
    double m_pi_2 = 0.0;
    double m_sig_1 = 0.0;
    double m_sig_2 = 0.0;
    double m_r = 0.0;
    std::vector<double> m_psi;

    std::string details() const {
      return "foobar";
    }
  };
}

std::ostream& operator <<(std::ostream& oss
  , const cfp::parameter<cfp::model<double, 2>>& p) {

  oss
    << "a_eta: "  << p.m_a_eta << "\n"
    << "a_mu:  "  << p.m_a_mu  << "\n"
    << "s_eta: "  << p.m_s_eta << "\n"
    << "s_mu:  "  << p.m_s_mu  << "\n"
    << "pi:    [" << p.m_pi_1  << ", " << p.m_pi_2 << "]\n"
    << "sig:   [" << p.m_sig_1 << ", " << p.m_sig_2 << "]\n"
    << "r:     "  << p.m_r     << "\n"
    << "psi:   ["
    ;

  std::string sep = "";
  for (auto& p : p.m_psi) {
    oss << sep << p;
    sep = ",";
  }
  
  oss
    << "]\n"
    ;

  return oss;
}
#endif
