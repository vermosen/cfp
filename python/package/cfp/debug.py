import numpy as np

class parameters:
  def __init__(self, a_eta, a_mu, s_eta, s_mu, r, pi, sigma, phi):
    self.a_eta = a_eta
    self.a_mu = a_mu
    self.s_eta = s_eta
    self.s_mu = s_mu
    self.r = r
    self.pi = pi
    self.sigma_ = sigma
    self.phi = phi

  def to_array(self):
    data = [
          self.a_eta
        , self.a_mu
        , self.s_eta
        , self.s_mu
        , self.r
    ] + self.pi + self.sigma_ + self.phi
    return np.array(data)
    
  def __repr__(self):
    retval = 'parameters('              \
            +   'a_eta=%f' % self.a_eta \
            + ', a_mu=%f' % self.a_mu   \
            + ', s_eta=%f' % self.s_eta \
            + ', s_mu=%f' % self.s_mu   \
            + ', r=%f)' % self.r        \
    
    return retval

  @property
  def a_0(self):
    return np.array([[self.a_eta, 0.0], [0.0, self.a_mu]])
  
  @property
  def a_1(self):
    return np.array([[1.0, 0.0], [0.0, self.a_mu]])
  
  @property
  def q_0(self):
    return np.array([[self.s_eta, 0.0], [0.0, self.s_mu]])
  
  @property
  def q_1(self):
    return np.array([[0.0, 0.0], [0.0, self.s_mu]])
  
  @property
  def sigma(self):
    return np.array([[self.sigma_[0], 0.0], [0.0, self.sigma_[1]]])

  @property
  def periodicity(self):
    return len(self.phi)

class model:

  def __init__(self, params):
    self.params = params
      
  def simulate(self, ndays, seed=None, debug=False):
    
    np.random.seed(seed)
    T = self.params.periodicity * ndays
    res = np.zeros(T)
    
    a_0 = self.params.a_0
    a_1 = self.params.a_1
    q_0 = self.params.q_0
    q_1 = self.params.q_1
    
    s1 = self.params.sigma
    x = np.random.multivariate_normal(self.params.pi, s1)
    c = np.array([1.0, 1.0]).reshape((1, 2))

    for tau in range(0, T):
      t = tau % self.params.periodicity
          
      if t == 0:
        x = a_0 @ x + np.random.multivariate_normal([0.0, 0.0], q_0)
      else:
        x = a_1 @ x + np.random.multivariate_normal([0.0, 0.0], q_1)

      res[tau] = c @ x + self.params.phi[t] + np.random.normal(0.0, self.params.r)
        
    return res
      
  def filter(self, y, debug=False):
    
    T = len(self.params.phi)

    # initial cmean ccov values
    cmean = np.array(self.params.pi).reshape((2, 1))
    
    # initial pmean pcov values
    ccov = self.params.sigma
    
    c = np.array([1.0, 1.0]).reshape((1, 2))
    rv = np.zeros(len(y))
    
    for t in range(0, len(y)):

      # new day ?
      if t % T == 0:
        amat = self.params.a_0
        qmat = self.params.a_1
      else:
        amat = self.params.q_0
        qmat = self.params.q_1

      pmean = amat @ cmean
      pcov  = amat @ ccov @ np.transpose(amat) + qmat
      kgain = pcov @ np.transpose(c) / (c @ pcov @ np.transpose(c) + self.params.r)
      cmean = pmean + kgain * (y[t] - self.params.phi[t % T] - c @ pmean)
      ccov  = pcov - kgain @ c @ pcov
      
      # the filtered signal
      rv[t] = c @ pmean + self.params.phi[t % T]

    return rv

  def smoother(self, y, debug=False):
    
    T = len(self.params.phi)

    # initial cmean ccov values
    cmean = [np.array(self.params.pi).reshape((2, 1))]
    
    # initial pmean pcov values
    ccov = [self.params.sigma]
    
    c = np.array([1.0, 1.0]).reshape((1, 2))
    rv = np.zeros(len(y))
    
    pmean = []; pcov = []
    for t in range(0, len(y)):

      # new day ?
      if t % T == 0:
        amat = self.params.a_0
        qmat = self.params.a_1
      else:
        amat = self.params.q_0
        qmat = self.params.q_1

      pmean.append(amat @ cmean[t])
      pcov.append(amat @ ccov[t] @ np.transpose(amat) + qmat)
      kgain = pcov[t] @ np.transpose(c) / (c @ pcov[t] @ np.transpose(c) + self.params.r)
      cmean.append(pmean[t] + kgain * (y[t] - self.params.phi[t % T] - c @ pmean[t]))
      ccov.append(pcov[t] - kgain @ c @ pcov[t])
    
    # initial smean values
    smean = cmean[-1]
    scov  = ccov[-1]
    
    for t in reversed(range(0, len(y))):
        
      if t % T == 0:
        amat = self.params.a_0
        qmat = self.params.a_1
      else:
        amat = self.params.q_0
        qmat = self.params.q_1
      
      l = ccov[t + 1] @ np.transpose(amat) @ np.linalg.inv(pcov[t])
      smean = cmean[t + 1] + l @ (smean - pmean[t])
      scov  = ccov[t + 1] + l @ (scov - pcov[t]) @ np.transpose(l)
      rv[t] = c @ smean + self.params.phi[t % T]
    
    return rv

  def em(self, y, tol = 1e-8, maxit=500, norm = None, debug = False):
    
    # initial conditions
    p = self.params
    diff = 1e10; n = 0
    
    # by default we use the l2 norm
    if norm is None:
      norm = lambda x, y : np.linalg.norm(x.to_array() - y.to_array(), 2)

    # main loop
    while (diff > tol and n < maxit):

      T = self.params.periodicity
      N = len(y); ndays = N / T
      c = np.array([1.0, 1.0]).reshape((1, 2))

      # initial cmean / ccov values
      cmean = [np.array(p.pi).reshape((2, 1))]
      ccov  = [p.sigma]


      # step 1: kalman filter
      pmean = []; pcov = []; kgain = []
      for t in range(0, len(y)):

        # new day ?
        if t % T == 0:
          amat = p.a_0
          qmat = p.a_1
        else:
          amat = p.q_0
          qmat = p.q_1

        pmean.append(amat @ cmean[t])
        pcov.append(amat @ ccov[t] @ np.transpose(amat) + qmat)
        denom = (c @ pcov[t] @ np.transpose(c) + p.r)
        kgain.append(pcov[t] @ np.transpose(c) / denom)
        cmean.append(pmean[t] + kgain[t] * (y[t] - p.phi[t % T] - c @ pmean[t]))
        ccov.append(pcov[t] - kgain[t] @ c @ pcov[t])

      # initial smean values
      smean = [cmean[-1]] * N
      scov  = [ccov[-1]]  * N
      sl    = [None]      * N
      
      # step 2: smoother
      for t in reversed(range(0, len(y) - 1)):

        if t % T == 0:
          amat = self.params.a_0
          qmat = self.params.a_1
        else:
          amat = self.params.q_0
          qmat = self.params.q_1

        try:
          inv = np.linalg.inv(pcov[t])
        except np.linalg.LinAlgError as ex:
          print('in iteration %s, step %s, matrix %s is singular !' % (n, t, pcov[t]))
          raise ex
            
        sl[t]    = ccov[t + 1] @ np.transpose(amat) @ inv
        smean[t] = cmean[t + 1] + sl[t] @ (smean[t + 1] - pmean[t])
        scov[t]  = ccov[t + 1] + sl[t] @ (scov[t + 1] - pcov[t]) @ np.transpose(sl[t])

      # step 3: now we compute each updated parameter
      xhat  = [None] * len(y)
      phat  = [None] * len(y)
      shat  = [None] * len(y)
      pphat = [None] * len(y)
      first = True
      
      for t in reversed(range(0, len(y))):
          
        if t % T == 0:
          amat = self.params.a_0
          qmat = self.params.a_1
        else:
          amat = self.params.q_0
          qmat = self.params.q_1
        
        xhat[t]  = smean[t]
        phat[t]  = scov[t] + xhat[t] @ np.transpose(xhat[t])
        
        if first:
          shat[t] = (np.array([[1.0, 0.0], [0.0, 1.0]]) - kgain[t] @ c) @ amat @ ccov[t + 1]
          first = False
        else:
          shat[t]  = ccov[t + 1] @ np.transpose(sl[t]) + sl[t] @ (shat[t + 1] - amat @ ccov[t + 1]) @ np.transpose(sl[t])
            
        # this one cannot be computed for t = 0
        if t > 0:
          pphat[t] = shat[t] + smean[t] @ np.transpose(smean[t - 1])
      
      # compute the updated parameters
      pi = xhat[0]
      sigma = phat[0] - pi @ np.transpose(pi)
      
      a_eta_n = 0; a_eta_d = 0
      a_mu_n = 0; a_mu_d = 0
      s_mu = 0; s_eta = 0
      r = 0; phi = [0.0] * self.params.periodicity
      
      for t in range(1, len(y)):
        if t % T == 0:
          a_eta_n += pphat[t][0][0]
          a_eta_d += phat[t][0][0]
    
        a_mu_n += pphat[t][1][1]
        a_mu_d += phat[t][1][1]
      
      a_eta = a_eta_n / a_eta_d
      a_mu  = a_mu_n / a_mu_d
      
      for t in range(1, len(y)):
        if t % T == 0:
          s_eta += phat[t][0][0] + a_eta ** 2 * phat[t-1][0][0] \
                  - 2 * a_eta * pphat[t][0][0]
    
        s_mu += phat[t][1][1] + a_mu ** 2 * phat[t-1][1][1] \
                  - 2 * a_mu * pphat[t][1][1]
      
      s_mu  /= T - 1
      s_eta /= N - 1
      
      for t in range(1, len(y)):
        phi[t % T] += y[t] - c @ xhat[t]
      
      phi = [f / T for f in phi]
      
      for t in range(0, len(y)):
        r += y[t] * y[t] + c @ phat[t] @ np.transpose(c)
        r -= 2 * y[t] * c @ xhat[t] + phi[t % T] * phi[t % T]
        r -= 2 * y[t] * phi[t % T] + 2 * phi[t % T] * c @ xhat[t]
      
      r /= N
      
      newp  = parameters(a_eta = a_eta
                  , a_mu = a_mu, s_eta = s_eta
                  , s_mu = s_mu, r = r, pi = [pi[0], pi[1]]
                  , sigma = [sigma[0][0], sigma[1][1]], phi = phi)
      
      diff = norm(newp, p)
      p     = newp
      n    += 1
        
    if debug:
      return p, diff, n
    else:
      return p

__all__ = [ 'parameters', 'model' ]