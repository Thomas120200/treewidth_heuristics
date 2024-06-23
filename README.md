# Treewidth heuristics and plotting

## Overview

This project contains a set of algorithms and graph operations, along with the necessary scripts to run experiments and visualize the results.

## Directory Structure

- **src/**: Contains all source files.
  - **.c files**: Contain the algorithms and graph operations.
  - **.py file**: Runs the experiments and handles plotting.
- **output/**: Contains the output data generated after running the program.

## Makefile Targets

The project is automated using a Makefile, which includes several targets for different tasks:

- **make run**: 
  - Runs the heuristics for `n = [10, 100, 1000]` and `p = [0.25, 0.5, 0.75]`, each with 100 experiments.
  - Plots the data after the experiments.
  - **Note**: This target will take a very long time (several hours).

- **make run_small**:
  - Runs the heuristics for `n = [10, 100]` and `p = [0.25, 0.5, 0.75]`, each with 10 experiments.
  - Plots the data after the experiments.
  - Use this target to quickly verify that the code runs correctly.

- **make plot**:
  - Shows the plot after the heuristics have been run.

- **make plot_precalc**:
  - Uses precalculated data from `./results.json` to show the plot for `n = [10, 100, 1000]` and `p = [0.25, 0.5, 0.75]`, each with 100 experiments.
  - This target saves time by using existing results.

## Usage

To run the project, use the following commands:

1. **Run all experiments and plot data:**
   ```sh
   make run
