
#include "cfp.h"

#include <pybind11/stl.h>
#include <pybind11/eigen.h>

#include <cfp/model/cfp.h>

using namespace pybind11::literals;

void init_cfp(pybind11::module& m) {

  auto sub = m.def_submodule("cfp");

  using model_type = cfp::cfp<double, 2>;
  using param_type = cfp::parameter<model_type>;
  using data_type  = Eigen::Matrix<double, Eigen::Dynamic, 1>;

  pybind11::class_<param_type>(sub, "parameters")
    .def(pybind11::init<>())
    .def_readwrite("a_eta", &param_type::m_a_eta)
    .def_readwrite("a_mu" , &param_type::m_a_mu )
    .def_readwrite("s_eta", &param_type::m_s_eta)
    .def_readwrite("s_mu" , &param_type::m_s_mu )
    .def_readwrite("r"    , &param_type::m_r    )
    .def_readwrite("psi"  , &param_type::m_psi  )
    .def_property("pi"
      , [](const param_type& p) {
        std::array<double, 2> arr = { p.m_pi_1, p.m_pi_2 };
        return arr;
      }
      , [](param_type& p, const std::array<double, 2>& arr) {
        p.m_pi_1 = arr[0];
        p.m_pi_2 = arr[1];
        return p;
      }
    )
    .def_property("sigma"
      , [](const param_type& p) {
        std::array<double, 2> arr = { p.m_sig_1, p.m_sig_2 };
        return arr;
      }
      , [&](param_type& p, const std::array<double, 2>& arr) {
        p.m_sig_1 = arr[0];
        p.m_sig_2 = arr[1];
        return p;
      }
    )
    .def("__repr__", [](const param_type& p) {
        std::ostringstream ss; 
        ss << p;
        return ss.str();
      }
    )
    .def("__str__", [](const param_type& p) {
        std::ostringstream ss; 
        ss << p;
        return ss.str();
      }
    )
    ;

    pybind11::class_<model_type>(sub, "model")
      .def(pybind11::init<const param_type&>())
      .def("filter", [](model_type& m, Eigen::Ref<const data_type> in) {
          data_type out = data_type::Constant(in.size(), NAN);
          m.filter(in, out);
          return out;
        }
      , pybind11::arg("data")
      )
      .def("smoother", [](model_type& m, Eigen::Ref<const data_type> in) {
          data_type out = data_type::Constant(in.size(), NAN);
          m.smoother(in, out);
          return out;
        }
      , pybind11::arg("data")
      )
      .def("emax", [](model_type& m
        , Eigen::Ref<const data_type> in
        , std::size_t maxstep
        , double tol) {
          
          cfp::parameter<model_type> out;
          m.emax2<cfp::recorders::type::none>(in, out, maxstep, tol);
          return out;
        }
      )
      // TODO: find a way to hide this function 
      // and call using some polymorphic override in py
      .def("emax_debug", [](model_type& m
        , Eigen::Ref<const data_type> in
        , std::size_t maxstep
        , double tol) {
          
          cfp::parameter<model_type> out;
          auto df = m.emax2<cfp::recorders::type::simple>(in, out, maxstep, tol);
          
          pybind11::dict d(
              "y"_a     = out
            , "a.eta"_a = df[0]
            , "a.mu"_a  = df[1]
            , "s.eta"_a = df[2]
            , "s.mu"_a  = df[3]
            , "pi.1"_a  = df[4]
            , "pi.2"_a  = df[5]
            , "sig.1"_a = df[6]
            , "sig.2"_a = df[7]
            , "r"_a     = df[8]
          );

          for (std::size_t i = 0; i < df.size() - 9; i++) {
            std::stringstream ss; ss << "psi." << i;
            d[ss.str().c_str()] = df[i + 9];
          }

          return d;
        }
      )
      .def("predict", [](model_type& m
        , Eigen::Ref<const data_type> in
        , std::size_t hrz) {
          data_type out = data_type::Constant(in.size(), NAN);
          m.predict(in, hrz, out);
          return out;
        }
      , pybind11::arg("data"), pybind11::arg("horizon")
      )
      ;
}