library(cfp)
library(testthat)
library(data.table)

options(digits=10)

tol <- 1e-7

# calibration result on SPY (2017-2019) data 
coefs       <- NULL
coefs$a.eta <- 0.93970117964
coefs$a.mu  <- 0.617656427391
coefs$s.eta <- 0.00611157182105
coefs$s.mu  <- 0.100157446738
coefs$pi    <- c(-0.0377453676475, 0.637412180444)
coefs$s1    <- c(9.49012643982e-05, 0.00182247858807)
coefs$r     <- 0.0670554645138
coefs$psi   <- c(  15.3322974898, 14.8914481022, 14.8960953411
                 , 14.7328251327, 14.717101214 , 14.600429107 , 14.5878141379
                 , 14.51710026  , 14.4863107795, 14.381950856 , 14.3423713871
                 , 14.2429064605, 14.2109154215, 14.1635064751, 14.2339929961
                 , 14.1522787054, 14.1947867672, 14.1869497863, 14.4385168913
                 , 14.3792550185, 14.4077635371, 14.4649051166, 14.624926768
                 , 14.7170306255, 15.0749366985, 15.9959231225)

test_that("test.kalman.filter", {
  
  params <- cfp.model.parameters(
      pi=c(1.0, 1.0)
    , s1=c(1.0, 1.0)
  )
  
  sim <- simulate(params, n=1000, seed=1234)
  sim$data[, f.yhat:= kalman(params, y, smooth=FALSE)]
  expect_equal(sim$data[.N, f.yhat], 0.727699456264327, tolerance=tol);
})

test_that("test.kalman.smoother", {
  
  params <- cfp.model.parameters(
    pi=c(1.0, 1.0)
    , s1=c(1.0, 1.0)
  )
  
  sim <- simulate(params, n=1000, seed=1234)
  sim$data[, s.yhat:= kalman(params, y, smooth=TRUE)]
  expect_equal(sim$data[1, s.yhat], 2.83643101423741, tolerance=tol);
})

test_that("test.cfp.profile", {
  
  x = seq(-1, 1, length.out = 26);
  
  params <- cfp.model.parameters(
      a.mu  = 0.3
    , a.eta = 0.3
    , s.eta = 1.0
    , s.mu  = 1e-4
    , psi   = x ^ 2.0 / 5.0
    , pi    = c(1.0, 1.0)
    , s1    = c(1.0, 1.0)
    , r     = 0.1)
  
  sim <- simulate(params, n=400, seed=1234)
  sim$data[, s.yhat:= kalman(params, y, smooth=TRUE)]
  sim$data[, f.yhat:= kalman(params, y, smooth=FALSE)]
  expect_equal(sim$data[1, s.yhat], 2.888115171, tolerance=tol);
  expect_equal(sim$data[.N, f.yhat], 0.4111250662, tolerance=tol);
})
