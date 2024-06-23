# Treewidth heuristics and plotting

## Overview

This project contains a set of algorithms and graph operations, along with the necessary scripts to run experiments and visualize the results.

## Directory Structure

- **src/**: Contains all source files.
  - **.c files**: Contain the algorithms and graph operations.
  - **.py file**: Runs the experiments and handles plotting.
- **output/**: Contains the output data generated after running the program: a folder for every experiment and a results.json file with all calculated treewidths. Each folder containins the graph, the elimination orderings, the treedecompositions and the treewidths for all three heuristics. 

## Makefile Targets

The project is automated using a Makefile, which includes several targets for different tasks:
- **make install**:
  - installs all dependencies from the requirements.txt file

- **make run**: 
  - Runs the heuristics for `n = [10, 50, 100, 200]` and `p = [0.25, 0.5, 0.75]`, each with 100 experiments.
  - Plots the data after the experiments.
  - **Note**: This target will take a very long time (several hours).

- **make run_small**:
  - Runs the heuristics for `n = [10, 100]` and `p = [0.25, 0.5, 0.75]`, each with 10 experiments.
  - Plots the data after the experiments.
  - Use this target to quickly verify that the code runs correctly.

- **make plot**:
  - this only hows the plot after the heuristics have been run (doesn't do experiments).

- **make plot_precalc**:
  - Uses precalculated data from `./results.json` to show the plot for `n = [10, 50, 100, 200]` and `p = [0.25, 0.5, 0.75]`, each with 100 experiments.
  - This target saves time by using existing results.

## Usage

To run the project, use the following commands:
1. **install dependencies**
   ```sh
   make install

1. **Run all experiments and plot data:**
   ```sh
   make run
2. **Run small set of experiments for verification:**
   ```sh
   make run_small
3. **Only plot the data after running experiments:**
   ```sh
   make plot
4. **Plot using precalculated data:**
   ```sh
   make plot_precalc

## Known Shortcomings
Unfortunately the min fill in heuristic takes a long time to execute on graphs with n > 200. The min fill in heuristic relies heavily on checking for existence of edges. Gprof shows that for a graph with 200 vertices the function edgeExists() is called 188 * 10⁶ times, consuming 92% of total execution time. Now the chosen datastructure for the graph plays a vital role for the performance of the min fill in heuristic. Unfortunately I started the project using an adjacency list representation making the check for existance of edges very expensive. I was not expecting that the edgeExists function would be the bottle neck. Instead the easy checking for neighbors and the small space complexity of adjacency lists on sparse graphs made me prefer it over the adjacency matrix. In hindsight, the matrix representation would have made more sense but I don't have the time to change it anymore. Therefore the make run command will only run experiments with up to 200 vertices, otherwise it would take days to finish. 
