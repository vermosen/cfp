#pragma once
#ifndef CFP_MATH_OPTIMIZATION_CRITERION_H
#define CFP_MATH_OPTIMIZATION_CRITERION_H

namespace cfp     {
namespace criteria {

    enum class type {
          functionTolerance             = 1
        , maxIterations                 = 2
        , stationaryFunctionValue       = 3
        , stationaryFunctionAccuracy    = 4
        , zeroGradientNorm              = 5
        , maxStationaryState            = 6
        , none                          = 0
    };
}

    // single criterion
    template <
        typename Problem
      , criteria::type Type
      , typename ... Args
    >
    class criterion;
}

#endif // CFP_MATH_OPTIMIZATION_CRITERION_H
