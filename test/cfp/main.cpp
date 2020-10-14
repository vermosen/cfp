
#include <gtest/gtest.h>

#include <cfp/model/parameters/cfp.h>
#include <cfp/model/simulations/cfp.h>

using namespace cfp;

TEST(cfp, set_simulation) {

  using model_type = cfp<double, 2>;
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
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
