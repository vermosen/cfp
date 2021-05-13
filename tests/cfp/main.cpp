
#include <gtest/gtest.h>

#include <cfp/model/parameters/cfp.h>
#include <cfp/model/simulations/cfp.h>

using namespace cfp;

TEST(unittest, set_simulation) {

  using model_type = model<double, 2>;
  parameter<model_type> params;
  params.m_a_eta = 0.4;
  params.m_a_mu  = 0.6;
  params.m_s_eta = 0.4;
  params.m_s_mu  = 0.6;
  params.m_pi_1  = 0.0;
  params.m_pi_2  = 0.0;
  params.m_sig_1 = 0.0;
  params.m_sig_2 = 0.0;
  params.m_r     = 0.0;
  params.m_psi = { 1.0, 1.0, 1.0, 1.0 };

  auto seed = 1234;
  std::int64_t sz = 1000;
  auto sim = simulation<model_type>(params, sz, seed);
  auto result = sim.next();

  ASSERT_FLOAT_EQ(result(sz - 1, 0), 0.56687903);
}

TEST(unittest, model_simulate) {

  using model_type = model<double, 2>;
  parameter<model_type> params;
  params.m_a_eta = 0.4;
  params.m_a_mu = 0.6;
  params.m_s_eta = 0.4;
  params.m_s_mu = 0.6;
  params.m_pi_1 = 0.0;
  params.m_pi_2 = 0.0;
  params.m_sig_1 = 0.0;
  params.m_sig_2 = 0.0;
  params.m_r = 0.0;
  params.m_psi = { 1.0, 1.0, 1.0, 1.0 };

  auto seed = 1234;
  std::int64_t sz = 1000;
  auto md = model<double, 2>(params);
  auto result = md.simulate(sz, seed);

  ASSERT_FLOAT_EQ(result(sz - 1, 0), 0.56687903);
}

TEST(unittest, model_simulate_v2) {

  using model_type = model<double, 2>;
  parameter<model_type> params, x0, out;

  params.m_a_eta = 0.30970117964    ;
  params.m_a_mu  = 0.337656427391   ;
  params.m_s_eta = 0.00611157182105 ;
  params.m_s_mu  = 0.100157446738   ; 
  params.m_pi_1  = 0.0377453676475  ;
  params.m_pi_2  = 0.637412180444   ;
  params.m_sig_1 = 9.49012643982e-05;
  params.m_sig_2 = 0.00182247858807 ;
  params.m_r     = 0.0670554645138  ;
  params.m_psi = { 15.3322974898, 14.8914481022, 14.8960953411
             , 14.7328251327, 14.717101214 , 14.600429107 , 14.5878141379
             , 14.51710026  , 14.4863107795, 14.381950856 , 14.3423713871
             , 14.2429064605, 14.2109154215, 14.1635064751, 14.2339929961
             , 14.1522787054, 14.1947867672, 14.1869497863, 14.4385168913
             , 14.3792550185, 14.4077635371, 14.4649051166, 14.624926768
             , 14.7170306255, 15.0749366985, 15.9959231225 };

  auto seed = 1;
  std::int64_t sz = params.m_psi.size() * 60;
  auto md = model<double, 2>(params);
  auto data = md.simulate(sz, seed);

  // check the last datapoint
  ASSERT_FLOAT_EQ(data(sz - 1, 0), 15.686205);

  // starting point for the optimization
  x0.m_a_eta = 0.4600193;
  x0.m_a_mu  = 0.4338574;
  x0.m_s_eta = 0.0004826461;
  x0.m_s_mu  = 0.0960447;
  x0.m_pi_1  = 0.4631345  ;
  x0.m_pi_2  = -0.8862641   ;
  x0.m_sig_1 = 1.182454e-06;
  x0.m_sig_2 = 5.822829e-05;
  x0.m_r = 0.08363227;
  x0.m_psi = {15.4187, 14.90523, 14.82741
    , 14.63593, 14.71093, 14.53411, 14.65409
    , 14.54779, 14.41814, 14.35303, 14.30254
    , 14.20725, 14.269, 14.15235, 14.27634
    , 14.17382, 14.22591, 14.1156, 14.30651
    , 14.37166, 14.36308, 14.43904, 14.62148
    , 14.73957,15.16542, 15.99318 };

  md = model<double, 2>(x0);
  auto rec = md.emax<cfp::recorders::type::simple>(data, out, 1000, 1e-10);
  ASSERT_FLOAT_EQ(out.m_a_eta,  0.30970117964);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
