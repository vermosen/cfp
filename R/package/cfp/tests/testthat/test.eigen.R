library(cfp)
library(testthat)
library(data.table)

tol <- 1e-10

test_that("eigen.self.adj.mult", {
	
	set.seed(1234)

	N <- 1000
	A <- matrix(complex(real = rnorm(N * N), imaginary = rnorm(N * N)), N)

	eigen.res <- cfp::selfadj_mult(A)
	r.res <- Re(A %*% Conj(t(A)))

	expect_equal(norm(eigen.res - r.res), 0.0, tolerance=tol)
})

# benchmark:
# library(microbenchmark)
# bk <- microbenchmark::microbenchmark(base = A %*% Conj(t(A)), eigen = cfp::selfadj_mult(A), times = 20L)
# autoplot(bk)
# bk
