
# 3 methods to get the acf

data <- ...

# method 1
res1 <- acf(data, lag.max=104, plot = FALSE)$acf
plot(res1)

# method 2
res2 <- ccf(data, data, plot=FALSE)$acf
res2 <- tail(res2, length(res2) / 2)
plot(res2)

# method 3
res <- data - mean(data)
res <- c(res, rep(0, length(res) - 1))  # pad to 2N - 1
res <- fft(res)
res <- res * Conj(res)
res <- Re(fft(res, inverse = T))
res <- res / res[1]
plot(res[1:104])