#pragma once

namespace cfp    {
namespace models {

  template <typename Child>
  class model {
  public:
    using repr_type = typename traits::representation<Child>::type;

  protected:
    template <typename, solvers::type> friend class solver;
    model(const repr_type&);
    repr_type representation() const;
  };
}}
// TODO