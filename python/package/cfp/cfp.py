import numpy as np
from ._cfp.cfp import parameters, model

"""@package cfp.cfp
Expose the function related to the Chen-Feng_Palomar model

More details.
"""
def simulate(p, size=1000):
  """sample a cfp dynamic

  The function create sample time-series
  """
  Qp = np.matrix([[p.s_eta, 0], [0.0, p.s_mu]])
  Ap = np.matrix([[p.a_eta, 0], [0.0, p.a_mu]])
    
  Qn = np.matrix([[0.0, 0.0], [0.0, p.s_mu]])
  An = np.matrix([[1.0, 0], [0.0, p.a_mu]])

  result = np.zeros(size)
  
  # initial values
  x      = np.matrix(p.pi)
  sig    = np.matrix(p.sigma)
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
