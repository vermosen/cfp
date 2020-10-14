import cfp.fft
import numpy as np

# compare the numbers with numpy's
sz = 10 ** 5
x = np.array(np.random.rand(sz)) + np.array(np.random.rand(sz)) * 1.j
x = np.array(x, dtype=np.complex128)
y = cfp.fft.fft_complex(x)

assert np.abs(np.linalg.norm(np.fft.fft(x) - y)) < 1e-7
