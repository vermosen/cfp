library(cfp)
library(wotan)
library(nanotime)
library(testthat)
library(data.table)


tol <- 1e-9

test_that("test.ar", {
	
    options(digits=20)
    df <- data.table(
      t=c(
          nanotime('2020-06-12T03:10:02.678580992+00:00')
        , nanotime('2020-06-12T03:10:02.678592256+00:00')
        , nanotime('2020-06-12T03:10:02.678597632+00:00')
        , nanotime('2020-06-12T03:10:02.778604032+00:00')
        , nanotime('2020-06-12T03:10:02.923208192+00:00'))
      , x=c(1.1, 1.2, 1.3, 1.4, 1.5))

    df[, lag.na := wotan::shift(t, x, by=11264)]
    df[, lag.fill := wotan::shift(t, x, by=11264, mode='fill')]
    df[, ar:= cfp::ewma(t, lag.na, halflife=1e9)]

    expect_equal(nrow(df), 5L);
	expect_equal(df[, ar], c(NA, 1.10000000000000, 1.10000000000000, 1.11339422950416, 1.14072852372207), tolerance=tol);
})