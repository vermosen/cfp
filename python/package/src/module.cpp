
#include "cfp.h"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(_cfp, m) {

  m.attr("have_eigen") = true;
  m.doc() = "this package implements the Chen-Feng-Palomar model";
  init_cfp(m);
}