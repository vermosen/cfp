library(cfp)
library(data.table)

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
sim <- simulate(params, n=100, seed=1234)
df <- em(params, col=sim$data[,y], max.step=10, tol=1e-10, debug=TRUE)      