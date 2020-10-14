#' Compute the ewm covariance of two columns
#' 
#' @description the ewm transform is such that: ...
#'
#' @family statistics
#' @param col1 a data.table column
#' @param col2 a data.table column
#' @param halflife the halflife of the weighting scheme
#' @param decay the decay to apply to the columns
#' @param bias [TODO] document
#' @param adjust [TODO] document 
#'
#' @return a column containing the transformed series
#'
#' @examples
#' library(data.table)
#'
#' dt <- data.table(x=rnorm(15), y=rnorm(15))
#' dt[,ewm.cov:=ewm.cov(x, y, decay=0.01, bias=TRUE)]
#'
#' @export
ewm.cov <- function(col1, col2, halflife = NULL, decay = NULL, bias = FALSE, adjust=TRUE) {

	if (!is.null(decay)) {
		(.Call(`_otos_ewm_cov`, PACKAGE = 'cfp', col1, col2, decay, bias, adjust))
	} else if (!is.null(halflife) && halflife > 0.0) {
		decay <- 1.0 - exp(log(0.5) / halflife)
		(.Call(`_otos_ewm_cov`, PACKAGE = 'cfp', col1, col2, decay, bias, adjust))
	} else {
		stop('either halflife or decay argument must be set !')
	}
}

#' Compute the exponentially-weighted moving variance of a column
#'
#' @family statistics
#' @param col a data.table column
#' @param halflife the halflife of the weighting scheme
#' @param decay the decay to apply to the columns
#' @param bias [TODO] document
#' @param adjust [TODO] document
#'
#' @return a column containing the transformed series
#'
#' @examples
#' library(data.table)
#'
#' dt <- data.table(x=rnorm(15))
#' dt[,ewm.var:=ewm.var(x, decay=0.01, bias=TRUE)]
#'
#' @export
ewm.var <- function(col, halflife = NULL, decay = NULL, bias = FALSE, adjust=TRUE) {
	(ewm.cov(col1=col, col2=col, halflife=halflife, decay=decay, bias, adjust))
}

#' Compute the exponentially-weighted moving standard deviation of a column
#'
#' @family statistics
#' @param col a data.table column
#' @param halflife the halflife of the weighting scheme
#' @param decay the decay to apply to the columns
#' @param bias [TODO] document
#' @param adjust [TODO] document
#'
#' @return a column containing the transformed series
#'
#' @examples
#' library(data.table)
#'
#' dt <- data.table(x=rnorm(15))
#' dt[,ewm.std:=ewm.std(x, decay=0.01, bias=TRUE)]
#'
#' @export
ewm.std <- function(col, halflife = NULL, decay = NULL, bias = FALSE, adjust=TRUE) {
	(sqrt(ewm.cov(col1=col, col2=col, halflife=halflife, decay=decay, bias=bias, adjust=adjust)))
}