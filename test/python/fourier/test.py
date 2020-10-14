import unittest

import numpy as np
from cfp.fft import flag, plan

class testFft(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(testFft, self).__init__(*args, **kwargs)
        
    def test_flag(self):
        
        f = flag(flag.estimate | flag.destroy_input)
        self.assertEqual(int(f), 65)
        f |= flag.patient
        self.assertEqual(int(f), 97)

    def test_plan(self):
        
        f = flag(flag.estimate | flag.conserve_memory)
        arr = np.array(np.random.rand(1000))
        arr.flags.writeable = True
        p = plan(arr, f)
        self.assertIsNotNone(p)

if __name__ == '__main__':
    unittest.main()