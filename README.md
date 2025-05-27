# nvtxR: R Bindings for NVIDIA NVTX Ranges

**nvtxR** provides simple R wrappers for NVIDIA’s NVTX (NVIDIA Tools Extension) API, allowing you to instrument your R code with NVTX ranges. This makes it easy to annotate CPU or GPU workloads with colored ranges that show up in profilers such as Nsight Systems or Nsight Compute.
By calling `push_range(name, cid)` and `pop_range()`, you can mark sections of your R code, assign them one of seven predefined colors, and visualize timing and nesting in a profiling session.

## Prerequisites
1.  **R (≥ 3.6.0)**
2.  **NVIDIA NVTX library**
    -   You must have NVTX headers (`nvToolsExt.h`) and library (`libnvToolsExt.so` on Linux) installed.
    -   Typically provided alongside the NVIDIA CUDA Toolkit, but you can also install the standalone NVTX SDK
3.  **C Compiler**
    -   `gcc` or `clang` on Linux
## Installation

Clone this repository and install the package in R:

```bash
git clone https://github.com/yourusername/nvtxR.git
cd nvtxR
R CMD build .
R CMD INSTALL nvtxR_0.1.0.tar.gz

```

> **Note**:
> 
> -   Edit `src/Makevars`  to point to where NVTX headers or libraries are located

If you prefer a one‐line install from GitHub (using **remotes** or **devtools**), you can do:

```r
# install.packages("remotes")   # if you don’t have it already
remotes::install_github("yourusername/nvtxR")

```


## Usage

Once installed, simply load the package and start annotating your code:

```r
library(nvtxR)

# Push an NVTX range named "my_task", using color ID 2 (colors[2 % 7])
push_range("my_task", 2)
Sys.sleep(0.3)   # Simulate some work
pop_range()

```

### Functions

-   **`push_range(name, cid)`**
    -   `name` (string): The label that will appear in the profiler UI.
    -   `cid` (integer): A color index. Internally, `cid % 7` selects one of seven ARGB colors.

-   **`pop_range()`**
    -   Pops the most recent NVTX range via `nvtxRangePop()`.

## Example: CPU “Work” with Loops and Sleeps

Below is a sample R script (`test_nvtx.R`) that you can run under Nsight Systems to see colored markers. Put this in a file or run interactively:

```r
library(nvtxR)

# A dummy CPU‐intensive function
cpu_work <- function(n = 1e6) {
  x <- 0
  for (i in seq_len(n)) {
    x <- x + sqrt(i)
  }
  invisible(x)
}

# 1) Multiple standalone ranges
for (i in 1:5) {
  push_range(paste0("step_", i), cid = i)
  Sys.sleep(0.1 + i * 0.05)   # Simulate varying workload
  pop_range()
}

# 2) Combine CPU work loops
for (i in 1:4) {
  push_range(sprintf("compute_loop_%d", i), cid = i + 2)
  cpu_work(5e5 + i * 5e5)
  pop_range()
}

# 3) Nested ranges
push_range("outer_section", cid = 0)
for (j in 1:3) {
  push_range(paste0("inner_", j), cid = j + 3)
  Sys.sleep(0.2)
  pop_range()
}
pop_range()

```

#### Profiling with Nsight Systems

```bash
# On Linux/macOS (assuming nsys is in your PATH):
nsys profile --trace=nvtx,cuda --stats=true \
             Rscript test_nvtx.R

```

-   Open the resulting `.qdrep` file in the Nsight Systems GUI.
    
-   You should see colored range annotations labeled “step_1”, “compute_loop_2”, “outer_section”, etc., along the timeline.
    

## Development: Build From Source

If you want to modify or rebuild the package from source:

1.  **Clone the repo**
    
    ```bash
    git clone https://github.com/yourusername/nvtxR.git
    cd nvtxR
    
    ```
    
2.  **Edit `src/Makevars` / `src/Makevars.win`**  
    Point `PKG_CFLAGS` and `PKG_LIBS` to the NVTX SDK on your system.
    
3.  **Build & Install**
    
    ```bash
    R CMD build .
    R CMD INSTALL nvtxR_<version>.tar.gz
    
    ```
    
4.  **Run Tests / Examples**
    
    ```r
    library(nvtxR)
    # Copy/paste the example from “Usage” above or run `test_nvtx.R`.
    
    ```

## License

This project is licensed under the **MIT License**.

Happy profiling! If you run into any issues, please open an issue on [GitHub](https://github.com/zalbanob/nvtxR/issues).
