// --------------------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/Dense_NN
// Lizenz: MIT (also for all self-developed included h and cpp files)
// Version 01.00, 28.03.2026
// Eigen-Version: 3.4.0
// cnpy: Copyright (C) 2011  Carl Rogers
// zlib: Copyright (C) 1995-2024 Jean-loup Gailly and Mark Adler
// C-Version: ISO-Standard C++20
// --------------------------------------------------------------------


# OrthoFlow  
SVD-based, modular feature engineering pipeline for reproducible machine learning workflows.

OrthoFlow is a lightweight, transparent C++ tool for data preprocessing,
scaling, SVD-based dimensionality reduction, and educational visualization.
It was developed to enable reproducible, traceable, and robust ML pipelines
without “magical” frameworks.

---

## ✨ Features

- **Modular Pipeline** (Load → Scale → Split → SVD → Visualize)
- **Multiple scalers**: Standard, MinMax, Robust, MaxAbs
- **Persistent history** (JSON-based)
- **SVD variants**: Jacobi, BDC
- **Automatic dimension reduction** via cumulative variance
- **GNUPlot visualization** optional
- **CLI-controlled parallelization**
- **Robust error handling**
- **Didactically clear code**, ideal for teaching and research

---

## 🚀 Installation

### Prerequisites
- C++20-compatible compiler  
- CMake  
- Eigen (>= 3.4)  
- OpenMP (optional, but recommended)  

### Build

```bash
mkdir build && cd build
cmake ..
make -j

---

## Using

Minimal example:
./orthoflow --file data.csv

Generating and Displaying the Scree Plot:
./orthoflow --file data.csv --svd-plot-mode gnuplot

Applying MinMax Scaling
./orthoflow --scaler minmax

⚡ Parallelization
OrthoFlow leverages the built-in multithreading support of its native library
to accelerate computationally intensive operations such as matrix multiplication and SVD.

Thread control via CLI

--threads auto     # uses all available CPU cores
--threads 1        # single-thread mode (e.g., Raspberry Pi)
--threads N        # specifies the number of threads

Example:
./orthoflow --file data.csv --threads 4

Behavior and Recommendations:

auto: 
Uses the number of physical cores. Recommended for desktop systems.

1 thread: 
Recommended for Raspberry Pi 4/5 or systems with limited cooling capacity.

N threads:  
For benchmarking or specialized environments.

Hyper-Threading and Thread Limitation:

OrthoFlow does not benefit from Hyper-Threading.
Therefore, OrthoFlow automatically limits the number of threads to the number of physical cores.

If --threads is greater than the hardware capacity, the value is automatically reduced
and a warning is issued.

👤 Author
Günter Faes — EigenNET Project
eigennet@faes.de
Creator of Dense_NN and OrthoFlow
Focus: transparent, educational ML tools in C++
MIT License