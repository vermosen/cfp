#' create a plot out of a data.table object
#'
#' the index is used as a 
#'
#' @title plot
#' @rdname plot
#' @family graphics
#' @param df a data.table object containing the data
#' @param cols the columns to display
#' @param title the title of the graph
#' @param secondary additional columns to attach to the secondary y axis
#' 
#' @return a plotly graph object
#'
#' @examples
#' date <- '2019-01-01'
#' cdr <- calendar('USD')
#'
#' res <- isWeekEnd(cdr, date)
#'
#' @export
plot.data.table <- function(df, cols, title = "", secondary=NULL, index='index') {
  
  pl <- plot_ly(data = df, x = names, type = 'scatter', mode = 'lines')
  
  ay <- list(
    title		= "",
    zeroline	= FALSE,
    showline	= TRUE,
    mirror		= "ticks",
    gridcolor	= toRGB("gray"),
    gridwidth	= 1,
    autotick	= FALSE,
    ticks		= "outside",
    linewidth	= 2,
    showgrid	= TRUE,
    dtick		= 5.0
  )
  
  for (r in cols) {
    pl <- pl %>% add_lines(x = as.formula(paste0("~`", index, "`"))
						 , y = as.formula(paste0("~`", r, "`"))
						 , name = r, yaxis = "ay")
  }
  
  ay2 <- NULL;
  
  if (!is.null(secondary)) {

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
    
    for (r in secondary) {
      pl <- pl %>% add_lines(x = as.formula(paste0("~`", index, "`"))
						   , y = as.formula(paste0("~`", r, "`"))
						   , name = r, yaxis = "y2")
    }
  }
  
  pl <- pl %>% 
	layout(title = title, yaxis = ay, yaxis2 = ay2, xaxis = list(title="time")) %>%
	config(showTips = FALSE)

  return(pl)
}