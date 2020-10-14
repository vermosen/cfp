import cfp.fft
import numpy as np

# compare the numbers with numpy's
sz = 10 ** 6
x = np.array(np.random.rand(sz), dtype=np.float64)
y = np.array(x[::-1], dtype=np.float64)

convolve_mine = cfp.fft.convolution_real(x, y)
convolve_np = np.fft.irfft(np.fft.rfft(x) * np.fft.rfft(y))

assert np.mean(np.abs(convolve_mine - convolve_np)) < 1e-7