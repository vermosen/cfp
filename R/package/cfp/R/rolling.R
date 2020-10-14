#' Compute a rolling-windowed estimate of the covariance of two variables
#'
#' @family statistics
#'
#' @param col1 a data.table column
#' @param col2 a data.table column
#' @param obs the number of observations to consider
#' @param period the sampling period of observations
#' @param fill default value to fill in case no computation occurs
#'
#' @export
rolling.cov <- function(col1, col2, obs = NULL, period=1, fill=NA) {
	(.Call(`_otos_rolling_cov`, PACKAGE = 'cfp', col1, col2, obs, period, fill))
}

#' Compute a rolling-windowed estimate of the variance
#'
#' @family statistics
#'
#' @param col a data.table column
#' @param obs the number of observations to consider
#' @param step subsampling of the passed data
#' @param fill default value to fill in case no computation occurs
#'
#' @export
rolling.var <- function(col, obs = NULL, step=1, fill=NA) {
	(.Call(`_otos_rolling_var`, PACKAGE = 'cfp', col, obs, step, fill));
}

#' Compute a rolling-windowed estimate of the standard deviation
#'
#' @family statistics
#'
#' @param col a data.table column
#' @param obs the number of observations to consider
#' @param period the sampling period of observations
#' @param fill default value to fill in case no computation occurs
#'
#' @export
rolling.std <- function(col, obs = NULL, period=1, fill=NA) {
	(sqrt(rolling.var(col, obs, period, fill)));
}