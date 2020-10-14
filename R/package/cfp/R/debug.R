#' throw an exception from Rcpp
#'
#' this (admittedly) dumb function is actually pretty useful. 
#' The Rcpp framework is silently failing when the C++ ABI is
#' not perfectly matched between the R installation and the shared
#' library built by this project. In some case, the incompatibility 
#' is fairly difficult to figure out from the runtime errors thrown.
#' Throwing (and catching) an exception between the shared lib and R
#' turns out to be the simplier way to figure out ABI misalignement.
#'
#' @family debug
#' @export debug.throw.exception
debug.throw.exception <- function() {
	(.Call('_otos_throw_exc', PACKAGE = 'cfp'));
}
