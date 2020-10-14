#pragma once
#ifndef OTOS_MODEL_CACHES_CFP_H
#define OTOS_MODEL_CACHES_CFP_H

#include <cfp/math/optimization/solver.h>

#include <cfp/model/cache.h>

namespace cfp    {

  template <
      typename T
    , int Size
  >
  class cfp;

  template <typename T, int Size>
  struct cache<cfp<T, Size>, caches::type::simple> {
  public:
    using matrix_type = typename cfp<T, Size>::matrix_type;
    using vrow_type   = typename cfp<T, Size>::vrow_type  ;
    using vcol_type   = typename cfp<T, Size>::vcol_type  ;
    using data_type   = typename cfp<T, Size>::data_type  ;

  public:
    cache() = delete;
    cache(std::size_t);

    friend cfp<T, Size>;

  private:
    std::size_t size() const { return m_t; }
    void reset() {}

  private:
    // filter
    std::vector<matrix_type> m_ccovs;
    std::vector<matrix_type> m_pcovs;
    std::vector<vcol_type> m_cmeans ;
    std::vector<vcol_type> m_pmeans ;
    std::vector<vcol_type> m_gains  ;

    // smoother
    std::vector<vcol_type>   m_smeans;
    std::vector<matrix_type> m_scovs ;
    std::vector<matrix_type> m_lt    ;

    // em
    std::vector<matrix_type> m_pcors;
    std::vector<matrix_type> m_ps   ;
    std::vector<vcol_type>   m_xs   ;

    std::size_t m_t;
  };

  template <typename T, int Size>
  inline cache<cfp<T, Size>, caches::type::simple>::cache(std::size_t t)
  : m_t(t) {

    m_ccovs.reserve(m_t + 1 );
    m_pcovs.reserve(m_t + 1 );
    m_cmeans.reserve(m_t    );
    m_pmeans.reserve(m_t    );
    m_gains.reserve(m_t     );

    m_lt.resize(m_t);
    m_scovs.resize(m_t);
    m_smeans.resize(m_t);

    m_pcors.resize(m_t);
    m_ps.resize(m_t);
    m_xs.resize(m_t);
  }
}

#endif
