#pragma once
#ifndef OTOS__TRAITS_POINT_H
#define OTOS__TRAITS_POINT_H

#include <tuple>

#include <cfp/math/rolling.h>
#include <cfp/math/traits/fwd/point.h>

#include <cfp/tags.h>

namespace cfp    {
namespace traits  {

  template <>
  struct point<rolling<tags::covariance>> {
    using type = std::tuple<double, double>;
  };

  template <>
  struct point<rolling<tags::variance>> {
    using type = double;
  };
}}

#endif