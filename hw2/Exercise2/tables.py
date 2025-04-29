import re
import os
import subprocess as sp

def generate_latex_table(thread_ids_data, level, chunk, nthreads):
	table_str = r"""\begin{minipage}[b]{0.28\linewidth}\centering
\begin{tabular}{|c|c|c|c|c|}  % 5 columns for i, j=0 to 3
\hline
\textbf{i} & \multicolumn{4}{c|}{\textbf{{j}}} \\ \cline{2-5}
 & \textbf{0} & \textbf{1} & \textbf{2} & \textbf{3} \\ \hline
"""

	for i in range(len(thread_ids_data)):  # For each row i
		table_str += rf"\textbf{{{i}}}"  # Add the row label (i) 
		for j in range(len(thread_ids_data[i])):  # For each column j
			thread_ids = thread_ids_data[i][j]
			# Join thread IDs into a string, with padding if necessary (e.g., 003, 001)
			cell_value = "".join(f"{tid:01d}" for tid in thread_ids)
			table_str += f" & {cell_value}"  # Add each cell value to the row
		table_str += r" \\ \hline" + "\n"  # End of row

	# End the table structure and return the string
	table_str += r"""\end{tabular}
\caption{level %d, chunk %d, threads %d (dynamic scheduling)}
\label{tab:%d_%d_%d}
\end{minipage}""" % (level, chunk, nthreads, level, chunk, nthreads)
	return table_str


def extract_thread_ids(output):

  thread_ids_data = [[[None for _ in range(3)] for _ in range(4)] for _ in range(5)]
  pattern = r"Thread=(\d+) did c\[i\]\[j\] for i=(\d+) , j=(\d+) and k=(\d+)"

  # Decode the output from bytes to string
  output_str = output.decode('utf-8')  

  for line in output_str.splitlines():  # Iterate over lines of the decoded string
    match = re.match(pattern, line)
    if match:
      tid, i, j, k = map(int, match.groups())
      thread_ids_data[i][j][k] = tid

  return thread_ids_data




if __name__ == "__main__":
	# Example data (replace with your actual thread IDs)
	for level in [1,2,3]:
		os.system(f"make clean")
		os.system(f"make LEVEL={level}")
		# chunk = 2
		for chunk in [2, 3, 5]:
			print(r"\begin{table}[H]")
			for nthreads in [2, 4, 10]:
				# nthreads = 2
				# Use subprocess to run the program
				result = sp.check_output(['build/ask2_p', str(nthreads), str(chunk)])
				thread_ids_data = extract_thread_ids(result)
				latex_table = generate_latex_table(thread_ids_data, level, chunk, nthreads)  
				print(latex_table)
				print("%\n%")
			print(r"\end{table}")
