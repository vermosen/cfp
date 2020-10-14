library(cfp)
library(testthat)
library(data.table)

tol <- 1e-7

test_that("test.fft.create.flag", {
	
	f1 <- cfp::create.fft.flag('(measure|estimate)')
	f2 <- cfp::create.fft.flag('(patient|estimate)')

	expect_equal(f1, '(measure|estimate)');
	expect_equal(f2, '(measure|patient|estimate)');
})

test_that("test.fft.build.plan", {
	
	set.seed(1234)

	data <- rnorm(10000)
	flag <- cfp::create.fft.flag('estimate')
	p    <- cfp::create.fft.plan(flag, train=data)

	df <- data.table(s.1=rnorm(10000))
	df[, f.1:=p$execute(s.1)]
	df[, s.2:=rnorm(10000)]
	df[, f.2:=p$execute(s.2)]

	expect_equal(df[.N/2 +1, f.2], 4.827524589+0.0i, tolerance=tol)
	expect_equal(df[.N/2 +2, f.2], 0.000000000+0.0i, torerance=tol)
})

test_that("test.fft.convolve", {
	
	set.seed(1234)
	data <- rnorm(10000)
	flag <- cfp::create.fft.flag('estimate')
	p <- cfp::create.fft.plan(flag, train=data)

	df <- data.table(s.1=rnorm(10000))
	df[, f.1 := p$execute(s.1)]
	df[, s.2 := rnorm(10000)]
	df[, f.2 := p$execute(s.2)]
	df[, conv := convolution(s.1, s.2, p)]

	expect_equal(df[ 1, conv], -35.64114273, tolerance=tol)
	expect_equal(df[.N, conv], -53.08890100, tolerance=tol)
})
