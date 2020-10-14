library(cfp)
library(testthat)
library(data.table)

tol <- 1e-7

test_that("test.ewm.variance", {
	
	set.seed(1234)

	df = data.table(x=rnorm(50))
	df[, ewm:=cfp::ewm.var(x, halflife = 5)]

	expect_equal(is.na(df[1, ewm]), TRUE)
	expect_equal(df[.N, ewm], 0.44431761, tolerance=tol)
	expect_equal(df[ 2, ewm], 1.1018627, tolerance=tol)
})

test_that("test.ewm.covariance", {
	
	set.seed(1234)

	df = data.table(x=rnorm(50), y=rnorm(50))
	df[, ewm:=cfp::ewm.cov(x, y, decay = 0.04)]

	expect_equal(is.na(df[1, ewm]), TRUE)
	expect_equal(df[.N, ewm], -0.0651068142, tolerance=tol)
	expect_equal(df[ 2, ewm],  0.9084777802, tolerance=tol)
})