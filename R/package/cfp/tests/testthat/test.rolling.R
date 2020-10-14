library(cfp)
library(testthat)
library(data.table)

test_that("test.rolling", {
	
	df <- data.table(y=runif(1000))
	df[, var:=cfp::rolling.var(y, obs=20, fill=-1.0)]

	# TODO
	expect_equal(TRUE, TRUE);
})