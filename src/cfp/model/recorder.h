#pragma once
#ifndef CFP_MATH_DETAILS_RECORDER_H
#define CFP_MATH_DETAILS_RECORDER_H

namespace cfp      {
namespace recorders {

  enum class type {
      simple = 1
    , none   = 0
  };
}

  /*!
   * @class recorder
   * @brief a recorder class 
   * @tparam Problem the problem type
   * @tparam Type the type of recorder
   */
  template<
      typename Problem
    , recorders::type Type
  > struct recorder;
}

#endif
