import unittest

import cfp
import numpy as np

class testCfp(unittest.TestCase):

    def __init__(self, *args, **kwargs):

        super(testCfp, self).__init__(*args, **kwargs)

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

        self.market_params = p

    def test_print_params(self):
        self.assertEqual(self.params.__repr__(), 
            '''a_eta: 1\na_mu:  0.4\ns_eta: 1e-07\ns_mu:  0.3\npi:    [0.3, 0.3]\nsig:   [0.1, 0.1]\nr:     0.4\npsi:   [1,1,1,1]\n''')

    def test_simulate(self):

        np.random.seed(1234)
        sim = cfp.simulate(self.params, size=20)

        self.assertAlmostEqual(sim[0 ], 1.89816178       )
        self.assertAlmostEqual(sim[19], 0.877137719126786)

    def test_filter(self):
        
        np.random.seed(1234)
        sim = cfp.simulate(self.market_params, size=1000)
        md = cfp.model(self.market_params)
        kf = md.filter(sim)

        self.assertEqual(len(kf), 1000)
        self.assertAlmostEqual(kf[0] , 15.9319643025965  )
        self.assertAlmostEqual(kf[-1], 14.146247907494123)
    
    def test_smoother(self):

        np.random.seed(1234)
        sim = cfp.simulate(self.market_params, size=1000)
        md = cfp.model(self.market_params)
        sm = md.smoother(sim)

        self.assertEqual(len(sm), 1000)
        self.assertAlmostEqual(sm[0] , 15.927989016176651)
        self.assertAlmostEqual(sm[-1], 14.188584761204664)

    def test_emax(self):

        np.random.seed(1234)
        sim = cfp.simulate(self.market_params, size=1000)
        md = cfp.model(self.market_params)
        res = md.emax(sim, 100, 1e-8)

        self.assertAlmostEqual(len(res.psi), 26)
        self.assertAlmostEqual(res.a_eta   , 0.8780664351205582   )
        self.assertAlmostEqual(res.a_mu    , 0.5843683622567167   )
        self.assertAlmostEqual(res.s_eta   , 0.0016218036289216205)
        self.assertAlmostEqual(res.s_mu    , 0.10553455705671508                           )
        self.assertAlmostEqual(res.pi      , [-0.11646425697054973, 0.5083643647969468]    )
        self.assertAlmostEqual(res.sigma   , [5.844641110702882e-05, 0.0013662016517574371])
        self.assertAlmostEqual(res.psi[0]  , 15.289625725386964                            )
        self.assertAlmostEqual(res.psi[25] , 15.862185130118702                            )

    def test_emax_debug(self):

        np.random.seed(1234)
        md = cfp.model(self.market_params)
        sim = cfp.simulate(self.market_params, size=100)
        df = md.emax_debug(sim, 10, 1e-10)

        self.assertEqual(len(df)              , 36                   )
        self.assertEqual(len(df['a.eta'])     , 10                   )
        self.assertAlmostEqual(df['a.eta'][0] , 0.9717456011583988   )
        self.assertAlmostEqual(df['a.eta'][9] , 1.017014876796524    )
        self.assertAlmostEqual(df['s.eta'][0] , 0.003830140840219957 )
        self.assertAlmostEqual(df['psi.25'][0], 16.114983829824222   )
        self.assertAlmostEqual(df['psi.25'][9], 16.289168581902302   )

    def test_predict(self):

        np.random.seed(1234)
        sim = cfp.simulate(self.market_params, size=1000)
        md = cfp.model(self.market_params)
        pred = md.predict(data=sim, horizon=10)

        self.assertEqual(len(pred), 1000)
        self.assertAlmostEqual(pred[0] , 14.309777001178249)
        self.assertAlmostEqual(pred[-11], 14.292866700911807)
        self.assertTrue(np.isnan(pred[-10]))

if __name__ == '__main__':
    unittest.main()