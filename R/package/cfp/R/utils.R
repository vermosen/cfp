
replace_dot_alias_inner = function(e) {
  
  if (is.call(e) && !is.function(e[[1L]])) {

    if (e[[1L]] == ".") e[[1L]] = quote(list)
    for (i in seq_along(e)[-1L]) if (!is.null(e[[i]])) e[[i]] = replace_dot_alias_inner(e[[i]])
  }

  e
}

replace_dot_alias <- function(e) {
  (replace_dot_alias_inner(substitute(e)))
}
