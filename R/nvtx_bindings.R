COLORS <- structure(
  as.integer(0:6),
  .Names = c("GREEN","BLUE","YELLOW","MAGENTA","CYAN","RED","WHITE")
)


GREEN   <- COLORS["GREEN"]
BLUE    <- COLORS["BLUE"]
YELLOW  <- COLORS["YELLOW"]
MAGENTA <- COLORS["MAGENTA"]
CYAN    <- COLORS["CYAN"]
RED     <- COLORS["RED"]
WHITE   <- COLORS["WHITE"]

nvtx_push_range <- function(name, cid) {
  stopifnot(is.character(name), length(name) == 1)
  if (is.character(cid)) {
    stopifnot(length(cid) == 1, cid %in% names(COLORS))
    cid <- COLORS[cid]
  } else {
    stopifnot(is.numeric(cid), length(cid) == 1)
    cid <- as.integer(cid)
  }
  invisible(.Call("R_nvtx_push_range", name, cid))
}


nvtx_pop_range <- function() {
  invisible(.Call("R_nvtx_pop_range"))
}
