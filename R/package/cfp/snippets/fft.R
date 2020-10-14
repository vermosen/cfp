
library(cfp)
library(data.table)

options(digits=10)
set.seed(1234)

data <- rnorm(10000)
flag <- cfp::create.fft.flag('estimate')
p <- cfp::create.fft.plan(flag, train=data)

df <- data.table(s.1=rnorm(10000))
df[, f.1:=p$execute(s.1)]
df[, s.2:=rnorm(10000)]
df[, f.2:=p$execute(s.2)]

df[, conv:= cfp::convolution(p, s.1, s.2)]
df[, conv.old:= cfp::convolution_old(s.1, s.2)]