
library(xts)
library(cfp)
library(arrow)
library(data.table)
library(WaveletComp)

folder = '/tmp/parquet'
filename = 'aapl.zstd.parquet'

df <- as.data.table(arrow::read_parquet(paste(folder, filename, sep='/')))

df[,c('time', 'vwap'):= NULL]
df[,mid:=(bid+ask)/2]
df[,datetime:=as.posix.ct(date, nanos_since_mid)]
setorder(df, datetime)

col <- 'mid'
day <- 20190320
#time.series.plot(df[date==day], col)
data <- as.xts.data.table(df[date==day,.(date = datetime, mid)])

# power spectrum
w.data <- analyze.wavelet(
      as.xts.data.table(df[date==day,.(date = datetime, mid)])
    , col, make.pval = FALSE)

# plot from waveletComp
wt.image(w.data, show.date = TRUE)

# using plotly
data <- w.data$Power
data <- data[,seq(1, ncol(data), by=20)]

# ~ same as 
pl <- plot_ly(z=data, type='heatmap')
pl
