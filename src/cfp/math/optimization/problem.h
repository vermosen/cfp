#pragma once
#ifndef CFP_MATH_OPTIMIZATION_PROBLEM_H
#define CFP_MATH_OPTIMIZATION_PROBLEM_H

namespace cfp {

  template<
      typename Func
    , typename Criteria
  >
  class problem {
  public:
    using arg_type = typename Func::arg_type;

    public:
      problem(
          const Func& func
        , const arg_type& guess
        , const Criteria& criteria);

      Criteria& criteria() { return m_criteria; }
      Func& function()     { return m_func; }

    private:
      Criteria m_criteria;
      arg_type m_guess;
      Func& m_func;
  };

  template <typename Func, typename Criteria>
  inline problem<Func, Criteria>::problem(
      const Func& func
    , const arg_type& guess
    , const Criteria& criteria) 
    : m_criteria(criteria)
    , m_guess(guess)
    , m_func(func) {
    
  }
}

#endif // CFP_MATH_OPTIMIZATION_PROBLEM_H