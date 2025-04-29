import pandas as pd
import matplotlib.pyplot as plt
import os

os.system(f"bash script.sh")
# Load data from the results CSV
def graphing(name):
    data = pd.read_csv(name)

    # Pivot data for plotting
    pivot_data = data.pivot(index='nthreads', columns='chunk', values='time')

    # Plot the graph
    pivot_data.plot(kind='bar', figsize=(10, 6), width=0.8)

    # Customize graph
    plt.title("Execution Time (msec) vs #threads", fontsize=14)
    plt.xlabel("#threads", fontsize=12)
    plt.ylabel("Time in msec", fontsize=12)
    plt.legend(title="Chunk Size", fontsize=10)
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    # Save the graph
    plt.savefig(f'execution_time_graph_{name}.png')
    plt.show()

graphing("results.csv")
graphing("results1.csv")
graphing("results2.csv")
