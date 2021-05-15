import unittest

import numpy as np

from cfp.debug import parameters, model

class testPycalc(unittest.TestCase):

    def __init__(self, *args, **kwargs):

        super(testPycalc, self).__init__(*args, **kwargs)

        # some variance scaling parameter 
        var = 1e-2

        self.params = parameters(
                  a_eta = 0.3
                , a_mu  = 0.9
                , s_eta = 0.1 * var
                , s_mu  = 0.3 * var
                , r     = var
                , pi    = [0.0, 0.0]
                , sigma = [0.1 * var, 0.1 * var]
                , phi   = [1.0, 0.4, 0.5, 0.6, 0.8]
        )

    def test_build_model_py(self):

        m = model(self.params)
        self.assertIsInstance(m, model)
        self.assertEqual(self.params.periodicity, 5)

    def test_simulate_py(self):

        ndays = 10
        m   = model(self.params)
        sim = m.simulate(ndays, seed=1234)
        self.assertEqual(len(sim), 50)
        self.assertAlmostEqual(sim[0] , 1.0319564482532337)
        self.assertAlmostEqual(sim[-1], 1.0704449057368397)

    def test_kalmanfilter_py(self):

        ndays = 10
        m   = model(self.params)
        sim = m.simulate(ndays, seed=1234)
        kf  = m.filter(sim)

        self.assertEqual(len(kf), 50)
        self.assertAlmostEqual(kf[0] , 1.0               )
        self.assertAlmostEqual(kf[-1], 0.8001052530811588)

    def test_kalmansmoother_py(self):

        ndays = 10
        m   = model(self.params)
        sim = m.simulate(ndays, seed=1234)
        sm  = m.smoother(sim)

        self.assertEqual(len(sm), 50)
        self.assertAlmostEqual(sm[0] , 1.0318732797290902)
        self.assertAlmostEqual(sm[-1], 0.8626356274047451)

    def test_em_py(self):

        ndays = 4
        m   = model(self.params)
        sim = m.simulate(ndays, seed=1234)
        em  = m.em(sim)

        self.assertEqual(len(em), 20)
        self.assertAlmostEqual(em[0] , 1.0318732797290902)
        self.assertAlmostEqual(em[-1], 0.8626356274047451)

if __name__ == '__main__':
    unittest.main()