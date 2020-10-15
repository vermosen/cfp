
#include "cfp.h"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(_otos, m) {

  m.attr("have_eigen") = true;

  m.doc() = "implement the Chen-Feng-Palomar model";

  init_cfp(m);
}