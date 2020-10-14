library(cfp)
library(data.table)
library(plotly)
library(parallel)

params <- cfp.model.parameters(
    alpha_eta = 0.3
  , alpha_mu  = 0.3
  , sigma_eta = 0.1
  , sigma_mu  = 0.2
  , sigma_1   = c(0.1, 0.2)
  , pi_1      = c(0.3, 0.3), r=0.1)

sims <- mclapply(X=runif(100), FUN=function(x){ simulate(params, n=1000, seed=x)}, mc.cores=12)

df <- rbindlist(mclapply(X=seq_along(sims), FUN=function(i) { 
  sim <- sims[[i]]
  sim$pi_1 <- matrix(c(.485, .605), nrow=2)
  df <- em(sim, col='y', nstep=2000, debug=TRUE)
  df[, sim.id:=i]
  (df)
}, mc.cores=12))

plot_ly(data=df[, .(pi.1=mean(pi.1), pi.2=mean(pi.2)), by=step]
        , y=~pi.1, type='scatter', mode='lines', name='pi.1') %>%
  add_trace(y=~pi.2, type='scatter', mode='lines', name='pi.2')
