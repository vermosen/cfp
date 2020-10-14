library(cfp)
library(data.table)
library(microbenchmark)

set.seed(1234)

N < -1000
A < -matrix(complex(real = rnorm(N * N), imaginary = rnorm(N * N)), N)

eigen.res < -cfp::selfadj_mult(A)
r.res < -Re(A % *%Conj(t(A)))

microbenchmark::microbenchmark(
	  base  = A % *%Conj(t(A))
	, eigen = cfp::selfadj_mult(A)
	, times = 20L
)

autoplot(bk)
bk