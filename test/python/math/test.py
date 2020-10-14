import unittest

import cfp.math
import numpy as np

class testMath(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(testMath, self).__init__(*args, **kwargs)
        pass

    def test_add_array(self):

        arr = np.array([1.0, 2.0, 3.0])
        res = cfp.math.add(arr, arr)
        self.assertTrue(np.allclose(res, np.array([2.0, 4.0, 6.0])))

    def test_sum_real(self):

        arr = np.array([1.0, 2.0, 3.0], dtype=np.float64)
        res = cfp.math.colsum(arr)
        self.assertAlmostEqual(res, 6.0)

    def test_sum_complex(self):

        arr = np.array([1.0 + 2.j, 2.0 + 1.j, 3.0 + 3.j], dtype=np.complex128)
        res = cfp.math.colsum(arr)
        self.assertAlmostEqual(np.abs(res), 8.48528137423857)

if __name__ == '__main__':
    unittest.main()