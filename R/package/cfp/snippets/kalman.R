
library(cfp)

params <- cfp.model.parameters.2()
sim <- simulate2(params, n=26*30, seed=1234)
res.2 <- cfp::em(sim$param, col=sim$data[, y], max.step=500, tol=1e-15, debug=FALSE, new=TRUE)