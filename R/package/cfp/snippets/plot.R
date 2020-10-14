library(plotly)
library(data.table)

plot <- function(x, ...) UseMethod("plot")

replace_dot_alias = function(e) {
  
  if (is.call(e) && !is.function(e[[1L]])) {

    if (e[[1L]] == ".") e[[1L]] = quote(list)
    for (i in seq_along(e)[-1L]) if (!is.null(e[[i]])) e[[i]] = replace_dot_alias(e[[i]])
  }

  (e)
}

plot.data.table <- function(df, axis1=NULL, axis2=NULL) {

  sub1 <- replace_dot_alias(substitute(axis1))
  #sub2 <- replace_dot_alias(substitute(axis2))

  cols1 <- colnames(`[`(df, , j=eval(sub1)))
  #cols2 <- colnames(`[`(df, , j=eval(sub2)))
  cols2 <- axis2
  

  pl <- plot_ly(data = df, x=names, type = 'scatter', mode = 'lines')
  
  ay <- list(
      title		  = ""
    , zeroline	= FALSE
    , showline	= TRUE
    , mirror		= "ticks"
    , gridcolor	= toRGB("gray")
    , gridwidth	= 1
    , autotick	= FALSE
    , ticks		  = "outside"
    , linewidth	= 2
    , showgrid	= TRUE
    , dtick		  = 5.0
  )
  
  for (c in cols1) {
    pl <- pl %>% add_lines(y = as.formula(paste0("~`", c, "`")), name = c, yaxis = "ay")
  }
  
  ay2 <- NULL;
  
  if (!is.null(axis2)) {
    
    ay2 <- list(
        title      = ""
      , zeroline   = FALSE
      , showline   = TRUE
      , mirror     = "ticks"
      , gridcolor  = toRGB("gray")
      , gridwidth  = 1
      , autotick   = FALSE
      , ticks      = "outside"
      , linewidth  = 2
      , showgrid   = TRUE
      , side       = "right"
      , overlaying = "y"
      , dtick      = 5.0
    )
    
    for (c in cols2) {
      pl <- pl %>% add_lines(y = as.formula(paste0("~`", c, "`")), name = c, yaxis = "y2")
    }
  }
  
  pl <- pl %>% layout(title = title, yaxis = ay, yaxis2 = ay2)
  (pl)
}

foo <- data.table(date=c(1, 3, 1), bar=c(1, 2, 3), baz=c(2, 2, 3), gzz=c(0.1, 0.3, -0.1))
setorder(foo, date)

# TODO: only the first expression in expending correctly -> fix
plot(foo, axis1=.(bar, baz), axis2=c('gzz'))

# or

plot(foo, axis1=.(bar, baz), axis2='gzz')