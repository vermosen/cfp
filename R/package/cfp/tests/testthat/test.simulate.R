library(cfp)
library(testthat)
library(data.table)

options(digits=10)

tol <- 1e-5

custom.p <- cfp::cfp.model.parameters(
            a.eta=0.93970117964
          , a.mu=0.617656427391
          , s.eta=0.00611157182105
          , s.mu=0.100157446738
          , pi = c(-0.0377453676475, 0.637412180444)
          , s1 = c(9.49012643982e-05, 0.00182247858807) 
          , r = 0.0670554645138
          , psi = c(  15.3322974898, 14.8914481022, 14.8960953411
                 , 14.7328251327, 14.717101214 , 14.600429107 , 14.5878141379
                 , 14.51710026  , 14.4863107795, 14.381950856 , 14.3423713871
                 , 14.2429064605, 14.2109154215, 14.1635064751, 14.2339929961
                 , 14.1522787054, 14.1947867672, 14.1869497863, 14.4385168913
                 , 14.3792550185, 14.4077635371, 14.4649051166, 14.624926768
                 , 14.7170306255, 15.0749366985, 15.9959231225)
          )

test_that("test.simulate.default", {
  
  seed <- 1234
  params <- cfp.model.parameters()
  y <- cfp::simulate2(params, n=1000, seed=1234)
  expect_equal(length(y), 1000, tolerance=tol);
  expect_equal(y[1], 1.610060757, tolerance=tol);
  expect_equal(y[1000], 0.2413101024, tolerance=tol);
  expect_equal(mean(y), 1.019927579, tolerance=tol);
})

test_that("test.simulate.custom", {
  
  seed <- 1234
  y <- cfp::simulate2(custom.p, n=1000, seed=1234)
  expect_equal(length(y), 1000, tolerance=tol);
  expect_equal(y[1], 16.14188, tolerance=tol);
  expect_equal(y[1000], 13.56532, tolerance=tol);
  expect_equal(mean(y), 14.50003, tolerance=tol);
})
