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

test_that("test.generate.parameters", {
  
  params <- cfp.model.parameters(psi=c(0.1, 2.0, 1.0, 1.4, 5.2))
  expect_equal(length(params$psi), 5);
})

test_that("test.simulate.model", {
  
  params <- cfp.model.parameters(
      pi = c(1.0, 1.0)
    , s1 = c(1.0, 1.0)
  )
  
  sim <- simulate(params, n=1000, seed=1234, debug=TRUE)
  expect_equal(nrow(sim$data), 1000);
  expect_equal(sim$data[.N, y], 0.7165439278, tolerance=tol);
})

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

test_that("test.cfp.em.debug", {
  
  params <- cfp.model.parameters(
      a.eta  = coefs$a.eta
    , a.mu   = coefs$a.mu
    , s.eta  = coefs$s.eta
    , s.mu   = coefs$s.mu
    , psi    = coefs$psi
    , pi     = coefs$pi
    , s1     = coefs$s1
    , r      = coefs$r)
  
  # create 100 simulations of 1 month
  sims <- lapply(X=seq(1, 20), FUN=function(x){ simulate(params, n=26*30, seed=x)})
  
  dfs <- rbindlist(lapply(X=seq_along(sims), FUN=function(i) {
    df <- cfp::em(sims[[i]]$param, col=sims[[i]]$data[, y], max.step=500, tol=1e-15, debug=TRUE)
    df[, sim.id:=i]
    (df)
  }))
  
  res <- last(dfs[, lapply(.SD, mean), by=step])
  
  expect_equal(res$a.eta , 0.750450677    , tolerance=tol);
  expect_equal(res$a.mu  , 0.5866784125   , tolerance=tol);
  expect_equal(res$s.eta , 0.001078956704 , tolerance=tol);
  expect_equal(res$s.mu  , 0.1116757578   , tolerance=tol);
  expect_equal(res$pi.1  , -0.03011767327 , tolerance=tol);
  expect_equal(res$pi.2  , 0.6956861149   , tolerance=tol);
  expect_equal(res$sig.1 , 3.393007438e-05, tolerance=tol);
  expect_equal(res$sig.2 , 0.0006836201539, tolerance=tol);
  expect_equal(res$r     , 0.07655693922  , tolerance=tol);
  expect_equal(res$psi.0 , 15.28878653    , tolerance=tol);
  expect_equal(res$psi.11, 14.17358133    , tolerance=tol);
  expect_equal(res$psi.14, 14.217404      , tolerance=tol);
  expect_equal(res$psi.19, 14.35191985    , tolerance=tol);
  expect_equal(res$psi.25, 15.95853746    , tolerance=tol);
})

test_that("test.cfp.em", {
  
  params <- cfp.model.parameters()
  sim <- simulate(params, n=26*30, seed=1234)
  res <- cfp::em(sim$parameters, col=sim$data[, y], max.step=500, tol=1e-9, debug=FALSE)
  
  expect_equal(res$a.eta , 0.28486858       , tolerance=tol);
  expect_equal(res$a.mu  , 0.4126159733	    , tolerance=tol);
  expect_equal(res$s.eta , 4.76502985e-05   , tolerance=tol);
  expect_equal(res$s.mu  , 0.6330123301     , tolerance=tol);
  expect_equal(res$pi.1  , 0.2994100138     , tolerance=tol);
  expect_equal(res$pi.2  , -2.813000984	    , tolerance=tol);
  expect_equal(res$sig.1 , 9.94859557e-05   , tolerance=tol);
  expect_equal(res$sig.2 , 0.0134016197     , tolerance=tol);
  expect_equal(res$r     , 0.5227220751     , tolerance=tol);
  expect_equal(res$psi[1] , 1.081784038     , tolerance=tol);
  expect_equal(res$psi[2],  1.071014739     , tolerance=tol);
  expect_equal(res$psi[3], 0.9385844613     , tolerance=tol);
  expect_equal(res$psi[4], 1.025295128      , tolerance=tol);
})


test_that("test.cfp.predict", {
  
  params <- cfp.model.parameters(
      a.eta  = coefs$a.eta
    , a.mu   = coefs$a.mu
    , s.eta  = coefs$s.eta
    , s.mu   = coefs$s.mu
    , psi    = coefs$psi
    , pi     = coefs$pi
    , s1     = coefs$s1
    , r      = coefs$r)
  
  sim <- simulate(params, n=1000, seed=1234)
  sim$data[, y.pred := predict(params, y, period=4)]
  
  expect_equal(sim$data[1L, y.pred], 14.77212599, tolerance=tol);
  expect_equal(is.na(sim$data[.N - 3, y.pred]), TRUE, tolerance=tol);
  expect_equal(sim$data[.N - 4, y.pred], 14.10147395, tolerance=tol);
})
