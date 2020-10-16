#' @export
predict <- function(x, ...) UseMethod("predict")

#' @export
simulate <- function(x, ...) UseMethod("simulate")

#' @export
kalman <- function(x, ...) UseMethod("kalman")

#' @export
em <- function(x, ...) UseMethod("em")

#' parameter set for the Chen-Feng-Palomar model
#'
#' @family cfp
#' @param a.eta first entry of the Ap matrix
#' @param a.mu last entry of the Ap matrix
#' @param s.eta first entry of the Qp matrix
#' @param a.mu last entry of the Qp matrix
#' @param psi psi vector
#' @param r noise
#' @param pi initial state
#' @param s1 initial variance
#' @export
cfp.model.parameters <- function(
    a.eta = 0.3
  , a.mu  = 0.3
  , s.eta = 1e-4
  , s.mu  = 1.0
  , psi=c(1.0, 1.0, 1.0, 1.0)
  , pi = NULL
  , s1 = NULL
  , r  = 0.1) {

  if (is.null(pi)) {
    pi <- c(a.eta, a.mu)
  }

  if (is.null(s1)) {
    s1 <- c(s.eta, s.mu)
  }

  params <- new(cfp_parameters)
  params$a.eta <- a.eta
  params$a.mu  <- a.mu
  params$s.eta <- s.eta
  params$s.mu  <- s.mu
  params$pi.1  <- pi[1]
  params$pi.2  <- pi[2]
  params$sig.1 <- s1[1]
  params$sig.2 <- s1[2]
  params$r     <- r
  params$psi   <- psi
  (params)
}

#' generate simulations for the Chen-Feng-Palomar model
#'
#' A details of simulate
#'
#' @title simulate: simulate a process
#' @family cfp
#' @rdname simulate
#' @param params a cfp.model.parameters object
#' @param n the size of the simulation to generate
#' @param seed the seed of the simulation
#' @param debug returns internal of the simulation
#' @export
simulate.Rcpp_cfp_parameters <- function(params, n=1000, seed=Sys.time(), debug=FALSE) {
  
  # state space rep
  # x_t = alpha * x_t-1 + w_t, w_t  ~ N(0, eta)
  # y_t  = x_t + nu_t        , nu_t ~ N(0, r  )
  
  set.seed(seed)
  
    # observation matrix 
  C <- matrix(c(1.0, 1.0), nrow=1)
  
  # periodic matrices
  Qp <- matrix(c(params$s.eta, 0.0, 0.0, params$s.mu), nrow=2)
  Ap <- matrix(c(params$a.eta, 0.0, 0.0, params$a.mu), nrow=2)
  
  # not periodic matrices
  Qn <- matrix(c(0.0, 0.0, 0.0, params$s.mu), nrow=2)
  An <- matrix(c(1.0, 0.0, 0.0, params$a.mu), nrow=2)
  
  # initial values
  pi <- matrix(c(params$pi.1, params$pi.2), nrow=2)
  s1 <- matrix(c(params$sig.1, 0.0, 0.0, params$sig.2), nrow=2)

  psi <- params$psi
  period <- length(psi)
  r <- params$r

  # starting parameters
  df <- data.table(
      w1 = rep(as.numeric(NA), n)
    , w2 = rep(as.numeric(NA), n)
    , nu = rep(as.numeric(NA), n)
    , x1 = rep(as.numeric(NA), n)
    , x2 = rep(as.numeric(NA), n)
    , y = rep(as.numeric(NA), n))
  
  # initial state
  x <- pi
  
  df[, nu := rnorm(n, 0, sqrt(r))]
  df[, x1 := x[1, 1]]
  df[, x2 := x[2, 1]]
  
  y <- (C %*% x) + psi[1] + df[1, nu]
  w <- mvrnorm(n=1, c(0.0, 0.0), Qp)
  
  set(df, 1L, 'y' , y  )
  set(df, 1L, 'w1', w[1]   )
  set(df, 1L, 'w2', w[2]   )
  
  for(i in seq(2, n)) {
    
    # compute w
    if(i %% period == 1) {
      w <- mvrnorm(n=1, c(0.0, 0.0), Qp)
    } else {
      w <- mvrnorm(n=1, c(0.0, 0.0), Qn)
    }
    
    # update x
    if (i %% period == 1) {
      x <- Ap %*% x + matrix(w, nrow=2)
    } else {
      x <- An %*% x + matrix(w, nrow=2)
    }
    
    # compute y
    y <- C %*% x + psi[(i-1) %% period + 1] + df[i, nu]
    
    set(df, i, 'y' , y)
    set(df, i, 'x1', x[1, 1])
    set(df, i, 'x2', x[2, 1])
    set(df, i, 'w1', w[1])
    set(df, i, 'w2', w[2])
  }
  
  if (debug) {
    (list('parameters'=params, 'data'=df))
  } else {
    (list('parameters'=params, 'data'=data.table(y=df[, y])))
  }
}

#' compute the kalman filter for the Chen-Feng-Palomar model
#'
#' @family cfp
#' @rdname kalman	
#' @param param a parameters object
#' @param col a data.frame column
#' @param smooth compute the smoother as well
#' @export
kalman.Rcpp_cfp_parameters <- function(params, col, smooth=FALSE) {

    if (smooth) {
		(smoother_cfp(params, col));
	} else {
		(filter_cfp(params, col));
	}
}

#' predict function for the cfp model
#'
#' @family cfp
#' @rdname predict
#' @param param a cfp model param object
#' @param col the data to predict
#' @param period number of period to predict
#' @export
predict.Rcpp_cfp_parameters <- function(params, col, period=1) {
    (predict_cfp(params, col, period));
}

#' compute the expectation-maximization estimator for the cfp model
#'
#' @family cfp
#' @rdname em
#' @param param a simulation object to represent the startup parameters
#' @param debug returns the parameter sequence 
#' @param tol convergence tolerance 
#' @export
em.Rcpp_cfp_parameters <- function(param, col, max.step=30, tol=1e-9, debug=FALSE) {
    
    if (debug) {
		(as.data.table(em_cfp_debug(param, col, max.step, tol)));
	} else {
		(em_cfp(param, col, max.step, tol));
	}
}

#' generate simulations for the Chen-Feng-Palomar model
#' @family cfp
#' @param params a cfp.model.parameters object
#' @param n the size of the simulation to generate
#' @param seed the seed of the simulation
#' @export
sim.cfp.2 <- function(sim, n=1000, seed=Sys.time()) {
    (simulate_cfp(params, n, seed));
}