# Write a script that generates the following plots:
# 1. A plot of the discrete function S(p, n) = (n+1) / (n/p + 1 + 2 * log(p) + 4)
#    for p = 1, 2, 4, 8, 16, 32, 64 and n = 64, 128, 256, 512, 1024.
#   The x-axis should be p, and there should be a different curve for each value of n with a different color
#  and a legend indicating which curve corresponds to which value of n.
# 2. The same plot for E(p, n) = S(p, n) / p.

# Plot the datapoints as dots, and connect them with lines.

import matplotlib.pyplot as plt
import numpy as np

# Define the functions
def S(p, n):
	return (n + 1) / (n / p + 1 + 2 * np.log(p) + 4)

def E(p, n):
	return S(p, n) / p

# Define the values of p and n
p_values = [1, 2, 4, 8, 16, 32, 64]
n_values = [64, 128, 256, 512, 1024]

plt.rcParams.update({'font.size': 20})


# Create the plots
plt.figure(figsize=(14,9))

plt.plot([1, 64], [1, 64], 'k--', label='Perfect speedup')

for n in n_values:
	S_values = [S(p, n) for p in p_values]
	plt.plot(p_values, S_values, label=f'n={n}', marker='o')

# Add grid
plt.grid()
plt.xlabel('# of processors')
plt.ylabel('Program speedup')
plt.legend()
plt.title('Speedup of the parallel algorithm')
plt.savefig('plots/speedup_plot.png')
plt.show()

plt.figure(figsize=(14,9))
for n in n_values:
	E_values = [E(p, n) for p in p_values]
	plt.plot(p_values, E_values, label=f'n={n}', marker='o')

# Add grid
plt.grid()

plt.xlabel('# of processors')
plt.ylabel('Program efficiency')
plt.legend()
plt.title('Efficiency of the parallel algorithm')
plt.savefig('plots/efficiency_plot.png')
plt.show()