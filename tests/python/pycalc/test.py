import unittest

import cfp
from cfp.debug import simulate, kalman_filter

import numpy as np

class testPycalc(unittest.TestCase):

    def __init__(self, *args, **kwargs):

        super(testPycalc, self).__init__(*args, **kwargs)

        t = 26
        # create params
        p = cfp.parameters()

        p.a_eta = 1.0
        p.a_mu  = 0.4
        p.s_eta = 1e-7
        p.s_mu  = 0.3
        p.r     = 0.4
        p.pi    = [0.3, 0.3]
        p.sigma = [0.1, 0.1]
        p.psi   = [1.0] * t
        self.params = p
        p = None

        p = cfp.parameters()
        p.a_eta = 0.93970117964
        p.a_mu  = 0.617656427391
        p.s_eta = 0.00611157182105
        p.s_mu  = 0.100157446738
        p.pi    = [-0.0377453676475, 0.637412180444]
        p.sigma = [9.49012643982e-05, 0.00182247858807]
        p.r     = 0.0670554645138
        p.psi   = [ 
                   15.3322974898, 14.8914481022, 14.8960953411
                 , 14.7328251327, 14.717101214 , 14.600429107 , 14.5878141379
                 , 14.51710026  , 14.4863107795, 14.381950856 , 14.3423713871
                 , 14.2429064605, 14.2109154215, 14.1635064751, 14.2339929961
                 , 14.1522787054, 14.1947867672, 14.1869497863, 14.4385168913
                 , 14.3792550185, 14.4077635371, 14.4649051166, 14.624926768
                 , 14.7170306255, 15.0749366985, 15.9959231225 
                ]

        self.params = p

    def test_simulate_py(self):

        sim = simulate(self.params, size=20, seed=1234)
        self.assertEqual(len(sim), 20)
        self.assertAlmostEqual(sim[0], 16.05404276777931)
        self.assertAlmostEqual(sim[-1], 13.891517406397963)

    def kalman_filter_py(self):

        # we simulate according to the same parameter set
        p = cfp.parameters()
        p.a_eta = 0.4
        p.a_mu = 0.3
        p.s_eta = 1e-4
        p.s_mu = 1e-4
        p.psi = [2.0, 1.7, 2.0]
        p.r = 1e-4
        sim = simulate(p, size=1000, seed=1234)
        kf = kalman_filter(p, sim)

if __name__ == '__main__':
    unittest.main()