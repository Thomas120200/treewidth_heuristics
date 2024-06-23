import os

import matplotlib.pyplot as plt
import subprocess
from dataclasses import dataclass
from argparse import ArgumentParser
import json
import numpy as np

heuristics = ["min_degree", "min_fill_in", "max_cardinality"]


def run_experiment(n: int, p: float, dir: str):
    print(f"Running experiment...\tn = {n}\tp = {p}")
    subprocess.run(["./main", str(n), str(p), dir])
    treewidth = {}

    for heuristic in heuristics:
        with open(f"{dir}/{heuristic}_treewidth.csv") as f:
            treewidth[heuristic] = int(f.read().strip())

    return treewidth


def main():
    # parse args
    parser = ArgumentParser()
    parser.add_argument('--run_experiments', action='store_true')
    parser.add_argument('--plot', action='store_true')
    parser.add_argument('--small', action='store_true')
    parser.add_argument('--plot_precalc', action='store_true')
    args = parser.parse_args()

    if args.run_experiments:
        if args.small:
            n = [10, 100]
            p = [0.25, 0.5, 0.75]
            n_experiments = 10
        else:
            n = [10, 50, 100, 200]
            p = [0.25, 0.5, 0.75]
            n_experiments = 100

        if not os.path.exists("output"):
            os.mkdir("./output")

        results = []
        for N in n:
            for P in p:
                for i in range(n_experiments):
                    path = f"./output/{N}_{P}_{i}"
                    os.mkdir(path)
                    results.append(run_experiment(N, P, path))
        json.dump(results, open("./output/results.json", "w"))

    elif args.plot or args.plot_precalc:
        if args.plot_precalc:
            results = json.load(open("./results_precalc.json"))
        else:
            results = json.load(open("./output/results.json"))
        min_degree = []
        min_fill_in = []
        max_cardinality = []
        for result in results:
            min_degree.append(result["min_degree"])
            min_fill_in.append(result["min_fill_in"])
            max_cardinality.append(result["max_cardinality"])

        min_degree_np = np.array(min_degree)
        min_fill_in_np = np.array(min_fill_in)
        max_cardinality_np = np.array(max_cardinality)

        min_degree_avg = np.mean(min_degree_np)
        min_degree_mean = np.median(min_degree_np)
        min_fill_in_avg = np.mean(min_fill_in_np)
        min_fill_in_mean = np.median(min_fill_in_np)
        max_cardinality_avg = np.mean(max_cardinality_np)
        max_cardinality_mean = np.median(max_cardinality_np)

        average = [min_degree_avg, min_fill_in_avg, max_cardinality_avg]
        median = [min_degree_mean, min_fill_in_mean, max_cardinality_mean]

        min_val = min(min(min_fill_in), min(min_degree), min(max_cardinality))
        max_val = max(max(min_fill_in), max(min_degree), max(max_cardinality))
        axis_limits = [min_val - 1, max_val + 1]

        plt.figure(figsize=(20, 10))

        s = 7
        linewidth = 0.75

        plt.subplot(2, 3, 1)
        plt.scatter(min_degree, min_fill_in, alpha=0.5, s=s)
        plt.plot(axis_limits, axis_limits, 'g-', linewidth=linewidth)
        plt.xlabel('Min Degree Width')
        plt.ylabel('Min Fill In Width')
        plt.title('Min Degree vs Min Fill In')
        plt.xlim(axis_limits)
        plt.ylim(axis_limits)

        plt.subplot(2, 3, 2)
        plt.scatter(min_degree, max_cardinality, alpha=0.5, s=s)
        plt.plot(axis_limits, axis_limits, 'g-', linewidth=linewidth)
        plt.xlabel('Min Degree Width')
        plt.ylabel('Max Cardinality Width')
        plt.title('Min Degree vs Max Cardinality')
        plt.xlim(axis_limits)
        plt.ylim(axis_limits)

        plt.subplot(2, 3, 3)
        plt.scatter(min_fill_in, max_cardinality, alpha=0.5, s=s)
        plt.plot(axis_limits, axis_limits, 'g-', linewidth=linewidth)
        plt.xlabel('Min Fill In Width')
        plt.ylabel('Max Cardinality Width')
        plt.title('Min Fill In vs Max Cardinality')
        plt.xlim(axis_limits)
        plt.ylim(axis_limits)

        # Bar plots
        plt.subplot(2, 3, 4)
        plt.bar(heuristics, average, color='skyblue')
        plt.title('Average Treewidth')
        plt.ylabel('Average Treewidth')

        plt.subplot(2, 3, 5)
        plt.bar(heuristics, median, color='lightgreen')
        plt.title('Median Treewidth')
        plt.ylabel('Median Treewidth')

        plt.subplot(2, 3, 6).axis('off')


        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    main()
