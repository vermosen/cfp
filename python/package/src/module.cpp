
#include "cfp.h"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(_otos, m) {

  m.attr("have_eigen") = true;

  m.doc() = "module for fast signal frequency analysis";

  init_cfp(m);
}