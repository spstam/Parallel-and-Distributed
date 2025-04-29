import matplotlib.pyplot as plt
import subprocess
import numpy as np
import os
import argparse

def plot_2d(t_mean,k_values,t_stdev,n,filename):
	#Make the arrays np so its faster for plotting
	t_mean=np.array(t_mean)
	k_values=np.array(k_values)
	t_stdev=np.array(t_stdev)

	# Plot the data with error bars representing standard deviation
	plt.rcParams.update({'font.size': 20})
	plt.figure(figsize=(14,9))
	plt.errorbar(k_values, t_mean, yerr=t_stdev, fmt="*-", capsize=5, color='red', ecolor='black')

	# Add labels and title
	plt.xlabel('Unrolling Factor (K)')
	plt.ylabel('Exeution Time (ms)')
	plt.title(f'Execution Time with respect to K (N={n})')
	plt.legend([f'N={n}'], loc='upper left')
	
	# Display the plot
	plt.grid(True)

	# Save the plot
	if not os.path.exists("plots"):
		os.makedirs("plots")
	count=0
	while True:
		imagefile = f"plots/{filename}_{n}_v{count}.png"
		if os.path.exists(imagefile):
			count+=1
		else:
			break
	plt.savefig(imagefile)

	plt.show()

def read_data(n,filename):
	filename = f"results/{filename}_{n}.dat"
	# Initialize lists to store the data
	t_mean=[]
	t_stdev=[]
	k_values=[]
	# Read the data from the file
	with open(filename, 'r') as file:
		file.readline()
		for line in file:
			try:
				k, time_mean, time_stdev = map(float, line.strip().split(", "))
				t_mean.append(time_mean)
				k_values.append(k)
				t_stdev.append(time_stdev)
			except ValueError:
				print(f"Skipping invalid line: {line.strip()}")
	for i in range(len(t_mean)):
		print(f"For n = {int(n)} and k = {int(k_values[i])}: mean_time = {t_mean[i]} (of {executions} executions) and stdev {t_stdev[i]}")
	return t_mean, k_values, t_stdev

if __name__ == "__main__":

	parser = argparse.ArgumentParser(description='Plot the data')
	parser.add_argument('--n', type=int, help='The value of n')
	parser.add_argument('--e', type=int, help='Number of Executions: How many times to execute the program for each k')
	parser.add_argument('--rerun', action='store_true', dest="rerun", help='Run the program again')
	parser.add_argument('--norm', type=str, help='Type of norm: "1" or "inf"')

	args = parser.parse_args()
	n = args.n
	executions=args.e
	rerun=args.rerun
	norm = args.norm
	#filenames
	filename=f"matrix_norm_{norm}"
	filename_norm_inf="matrix_norm_inf"

	if not os.path.exists(f"results/{filename}_{n}.dat"):
		rerun = True
	
	if rerun:
		print("Running the program...")
		#run norm1
		exit_status = os.system(f"./build/{filename}.bin {executions} {n}")
		print(f"./build/{filename}.bin {executions} {n}")
		if exit_status!=0:
			print("error running")
			exit(1)
	
	t_mean, k_values, t_stdev = read_data(n,filename)
	
	plot_2d(t_mean, k_values, t_stdev,n,filename)

