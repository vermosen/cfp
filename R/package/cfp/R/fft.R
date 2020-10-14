
#' build a fftw flag
#'
#' @family fft
#' @export
create.fft.flag <- function(str='measure') {
    (otos_fft_flag(str));
}

#' build an estimation window object
#'
#' @family fft
#' @export
create.fft.window <- function(type, ...) {
    if(type == 'cosine')
        (new(otos_fft_window_cosine, ...))
    else {
        stop('unknown window type passed');
    }
}

#' build a 1d real fftw plan
#'
#' @family fft
#' @export
create.fft.plan <- function(flag=NULL, size=NULL, train=NULL) {
    
    if (is.null(flag)) {
      flag <- create.fft.flag('measure')
    }

    if (is.null(train)) {
        if(is.null(size)) {
            stop('must supply either size argument or train sample');
        } else {
            (new(otos_fft_plan, rep(0.0, size), flag));
        }
    } else {
        if (!is.null(size)) {
            warning('train arg precedes arg size so size will be ignored ...');
        }
        (new(otos_fft_plan, train, flag));
    }
}

#' compute the convolution of two 1d real arrays using the fft
#'
#' @family fft
#' @rdname convolution
#' @param x a time series
#' @param y a second time series
#' @param plan an optional real plan
#' @rdname convolution
#' @export
convolution <- function(x, y, plan=NULL) {
    
    if (length(x) != length(y)) {
        stop('x and y must have identical size')
    }

    if (is.null(plan)) {
        plan <- create.fft.plan(size=length(x))
    }

    (otos_convolve_real(x, y, plan))
}

#' compute the welsh transformation for the time series passed
#'
#' @family fft
#' @rdname welsh
#' @param x a time series
#' @param y a second time series
#' @param plan an optional real plan
#' @rdname convolution
#' @export
welch <- function(x, y, plan=NULL) {
    
    if (length(x) != length(y)) {
        stop('x and y must have identical size')
    }

    if (is.null(plan)) {
        plan <- create.fft.plan(size=length(x))
    }

    (otos_convolve_real(x, y, plan))
}