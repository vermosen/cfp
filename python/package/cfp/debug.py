import numpy as np

from ._cfp.core import parameters

def simulate(p, size=1000, seed =-1):
  """
  sample a dynamic wrt. the cfp parameters passed - debug version
  """
  np.random.seed(seed)

  Qp = np.array([[p.s_eta, 0], [0.0, p.s_mu]])
  Ap = np.array([[p.a_eta, 0], [0.0, p.a_mu]])
    
  Qn = np.array([[0.0, 0.0], [0.0, p.s_mu]])
  An = np.array([[1.0, 0], [0.0, p.a_mu]])

  result = np.zeros(size)
  
  # initial values
  x      = np.array(p.pi)
  sig    = np.array(p.sigma)
  psi    = p.psi
  period = len(psi)
  r      = np.sqrt(p.r)
  
  for i in range(0, size):

    result[i] = x.sum() + psi[i % period] + np.random.normal(0, r, 1)
    
    # update x, sigma
    if (i+1) % period == 0:
      w = np.random.multivariate_normal(np.zeros(2), Qp, 1)
      x = x * Ap + w
    else:
      w = np.random.multivariate_normal(np.zeros(2), Qn, 1)
      x = x * An + w
  
  return result

def kalman_filter(p, y, debug=False):

  t = len(p.psi)
  y_h = np.zeros(len(y))
  
  x_tt = p.pi
  s_tt = p.sigma
  
  for i in range(0, len(y)):
    
    if i % t == 0:
      A = np.array([[p.a_eta, 0.0], [0.0, p.a_mu]])
      Q = np.array([[p.s_eta, 0.0], [0.0, p.s_mu]])
    else:
      A = np.array([[1.0, 0.0], [0.0, p.a_mu]])
      Q = np.array([[0.0, 0.0], [0.0, p.s_mu]])
  
    x_ts = A @ x_tt
    s_ts = A @ s_tt @ A.transpose() + Q
  return y_h

__all__ = [ 'simulate', 'kalman_filter' ]