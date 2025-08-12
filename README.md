# Wavelet Forest

This repository contains a **Test implementation** of the **Wavelet Forest** data structure.  
The code is primarily intended for **experimental evaluation** and **concept demonstration**, rather than production use.

## Overview

A **Wavelet Forest** is a variant of the **Wavelet Tree** that improves **access locality** by dividing the data into fixed-size blocks, storing a separate wavelet tree for each block.  
This approach can reduce cache misses and improve query performance, particularly for large datasets or large alphabets.

The idea builds on **fixed-block compression boosting** by Kärkkäinen and Puglisi (2011), but focuses on performance gains from **better cache behavior** rather than compression.

In our experiments, Wavelet Forests achieved **up to 2.5× speedup** over standard Wavelet Matrices for random access queries, mainly due to reduced L3 cache misses.

## Features

- Simple C++ test code using [SDSL-lite](https://github.com/simongog/sdsl-lite)
- Implements both:
  - Standard Wavelet Matrix (baseline)
  - Wavelet Forest with adjustable block sizes
- Designed for **performance testing**

## Usage

1. **Build** the project (requires `g++` and `sdsl-lite` installed).
2. **Run**:
   - First, generate the random input file (8 MB).
   - Then, run the wavelet forest test program.
3. **Compare** performance metrics for Wavelet Matrix vs. Wavelet Forest.

Example build:
```bash
mkdir build
cd build && cmake .. && make

# Step 1: Generate an 8 MB random input file
./random_file_generator

# Step 2: Run the wavelet forest test
./wavelet_forest_test
```

## Reference
```
@InProceedings{10.1007/978-3-031-72200-4_14,
author="Hong, Aaron
and Boucher, Christina
and Gagie, Travis
and Li, Yansong
and Zeh, Norbert",
editor="Lipt{\'a}k, Zsuzsanna
and Moura, Edleno
and Figueroa, Karina
and Baeza-Yates, Ricardo",
title="Another Virtue of Wavelet Forests",
booktitle="String Processing and Information Retrieval",
year="2025",
publisher="Springer Nature Switzerland",
address="Cham",
pages="184--191",
abstract="A wavelet forest for a text T[1..n] over an alphabet {\$}{\$}{\backslash}sigma {\$}{\$}$\sigma$takes {\$}{\$}n H{\_}0 (T) + o (n {\backslash}log {\backslash}sigma ){\$}{\$}nH0(T)+o(nlog$\sigma$)bits of space and supports access and rank on T in {\$}{\$}O ({\backslash}log {\backslash}sigma ){\$}{\$}O(log$\sigma$)time. K{\"a}rkk{\"a}inen and Puglisi (2011) implicitly introduced wavelet forests and showed that when T is the Burrows-Wheeler Transform (BWT) of a string S, then a wavelet forest for T occupies space bounded in terms of higher-order empirical entropies of S even when the forest is implemented with uncompressed bitvectors. In this paper we show experimentally that wavelet forests also have better access locality than wavelet trees and are thus interesting even when higher-order compression is not effective on S, or when T is not a BWT at all.",
isbn="978-3-031-72200-4"
}
```
