push_range <- function(name, cid) {
  stopifnot(is.character(name), length(name) == 1)
  stopifnot(is.numeric(cid), length(cid) == 1)
  invisible(.Call("R_push_range", name, as.integer(cid)))
}

pop_range <- function() {
  invisible(.Call("R_pop_range"))
}
