# cfp.R

#' cfp
#'
#' cfp implement the Chen-Feng-Palomar model
#'
#'  Imports
#' @import data.table
#' @import MASS
#' @useDynLib cfp, .registration=TRUE
"_PACKAGE"

# enable the data.table semantic
.datatable.aware = TRUE;

# load the internal environment with related static data
.pkgenv <- new.env(parent=emptyenv())

# the package init functions
.onLoad <- function(libname, pkgname) {}

.onAttach <- function(libname, pkgname) {
  packageStartupMessage("loading cfp v.", packageDescription("cfp")$Version, " ...")
}

# extend S4 classes here ...
setMethod('show', 'Rcpp_cfp_parameters', function(object) {

    cat('a.eta:', object$a.eta, '\n')
    cat('a.mu: ', object$a.mu , '\n')
    cat('s.eta:', object$s.eta, '\n')
    cat('s.mu: ', object$s.mu , '\n')
    cat('r:    ', object$r    , '\n')
    cat('psi:  ', object$psi  , '\n')
    cat('x_1:  ', object$pi.1 , ',', object$pi.2, '\n')
    cat('s_1:  ', object$sig.1, ',', object$sig.2, '\n')
})