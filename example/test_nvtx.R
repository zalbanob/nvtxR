library(nvtxR)

simulate_work <- function(name, cid, duration = 0.2) {
  push_range(name, cid)
  Sys.sleep(duration) 
  pop_range()
}

# Simulate 5 named work blocks with different color IDs
for (i in 1:5) {
  simulate_work(paste0("step_", i), cid = i, duration = 0.1 + 0.05 * i)
}

# Optional: simulate nested ranges
push_range("outer_loop", 100)
for (i in 1:3) {
  simulate_work(paste0("inner_step_", i), cid = i + 10, duration = 0.2)
}
pop_range()
