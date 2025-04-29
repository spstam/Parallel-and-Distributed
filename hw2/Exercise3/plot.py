import subprocess
import os
import numpy as np

table_header = """ % Table
\\begin{table}[]
\\begin{tabular}{|l|ll|}
\\hline
\\multirow{2}{*}{\\textbf{Nthreads}} & \\multicolumn{2}{c|}{\\textbf{Execution Time}}                                \\\\ \\cline{2-3} 
                                   & \\multicolumn{1}{c|}{\\textbf{Simple}} & \\multicolumn{1}{c|}{\\textbf{Nested}} \\\\ \\hline
"""

table_footer = """\\end{tabular}
\\end{table}
"""

def getRow(threads, simple_time, nested_time):
	return f"{threads}" + " & " + "\\multicolumn\\{1}{l|}{" + f"{simple_time}" + "}" + " & " + f"{nested_time}"	 + " \\\\ \\hline\n"

N = 10000
maxIter = 200
tol = 1e-6
out_dir = f"output_{N}"
times = 4
time = []
threads_array = [1, 2, 4, 5, 6, 8, 10, 12, 16, 20, 32]
serial_time = 0
serial_time_times = 0

for i, jacobi_type_arg in enumerate(["SIMPLE_JACOBI", "NESTED_JACOBI"]):
	time.append([])
	for j, threads in enumerate(threads_array):
		# 0. Set the number of threads as an environment variable
		os.environ["OMP_NUM_THREADS"] = str(threads)

		# 1. Compile with make
		subprocess.run(["make", "clean"], stdout=subprocess.PIPE)
		make_cmd = ["make"]
		if jacobi_type_arg == "NESTED_JACOBI":
			make_cmd.append("NESTED_JACOBI=1")
		elif j == 0:
			make_cmd.append("SERIAL=1")
		subprocess.run(make_cmd, stdout=subprocess.PIPE)
		os.makedirs(out_dir, exist_ok=True)

		# 2. Run the program
		time[i].append([])
		if jacobi_type_arg == "SIMPLE_JACOBI":
			times = 5
		elif threads > 8:
			times = 3
		for t in range(times):
			print("Running with", threads, "threads and ", jacobi_type_arg)
			output = subprocess.check_output(["./build/ask3", str(N), str(maxIter), str(tol)])
			output = output.decode("utf-8")
			of = f"{jacobi_type_arg}_t{threads}_{t}.out"
			with open(f"{out_dir}/{of}", "w") as f:
				f.write(output)

			for line in output.split("\n"):
				if "Serial code execution time" in line:
					serial_time += float(line.split(" ")[-1])
					serial_time_times += 1
				if "Parallel code execution time" in line:
					time[i][j].append(float(line.split(" ")[-1]))
					print("\tParallel code execution time: ", time[i][j][-1])
				if "Number of threads:" in line:
					if threads != int(line.split(" ")[-1]):
						print("\tError: Number of threads is not correct")
				if "Parallel code error:" in line:
					print("\tParallel code error: ", line.split(" ")[-1])
		time[i][j] = np.array(time[i][j])

serial_time /= serial_time_times

# 3. Write the results in a latex table
with open(f"table_{N}.tex", "w") as f:
	f.write(table_header)
	f.write(getRow("Serial", str(serial_time), "-"))
	for j, threads in enumerate(threads_array):
		f.write(getRow(threads, time[0][j].mean(), time[1][j].mean()))
	f.write(table_footer)