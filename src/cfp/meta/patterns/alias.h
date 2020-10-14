#pragma once

namespace cfp    {
namespace meta    {
namespace details {

  template<
      typename L
    , typename R
  >
  struct alias_cast_t {

    static_assert(sizeof(L) == sizeof(R)
      , "Cannot cast types of different sizes");

    union {
      L m_left  ;
      R m_right ;
    };
  };
}

  template<
      typename L
    , typename R
  >
  inline L alias_cast(R raw) {
    details::alias_cast_t<L, R> ac; // clear -Werror=strict-aliasing
    ac.m_right = raw;
    return ac.m_left;
  }
}}
