
#' Wavelet Filterbank Structure Initialization
fwtinit <- function(wavelet) {
	#https://github.com/ltfat/ltfat/blob/master/wavelets/fwtinit.m

}

#' compute the Fast Wavelet Transform (fwt)
#'
#' @family wavelet
#' @export fwt
fwt <- function(input, wavelet, J, boundary='periodic') {
   
  if (!is.element(boundary, c('periodic', 'zero', 'even', 'odd'))) {
	stop('boundary argument must be one of "periodic", "zero", "even", "odd"');
  }

  w = fwtinit(wavelet);
}