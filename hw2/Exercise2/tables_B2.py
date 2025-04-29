import os
import subprocess as sp


def generate_latex_table(thread_ids_data, level, chunks, thread_nums):
	# Start the table structure
	chunks_hdr = ' '.join([' & \\textbf{'+ str(chunk) +'}' for chunk in chunks])
	table_str = rf"""\begin{{table}}[H]
\centering
\begin{{tabular}}{{|c|c|c|c|c|}}  % 5 columns for i, j=0 to 3
\hline
\textbf{{Nthreads / Chunks}} {chunks_hdr} \\
\hline """
	# Loop over the rows (i) and columns (j) of the thread_ids_data
	for i, row in enumerate(thread_ids_data):
		table_str += rf"\textbf{'{'+str(thread_nums[i])+'}'}"  # Add the row label (i)
		for cell in row:
			cell_value = ", ".join(map(str, cell)) if cell else "N/A"
			table_str += f" & {cell_value}"  # Add each cell value to the row
		table_str += r" \\ \hline" + "\n"  # End of row

	# End the table structure and return the string
	table_str += rf"""\end{{tabular}}
\caption{{Performance Metrics (time, speedup, efficiency) for Matrix Multiplication (Level={level})}}
\label{{tab:ex2_table_l{level}}}
\end{{table}}"""
	return table_str


def extract_thread_ids(thread_ids_data, output, nthreads, chunk_idx):
    serial_time = 0.849  # Example constant for computation

    # Decode the output from bytes to string
    time = float(output.decode('utf-8').strip())  # Convert the program's output to float
    # Store the calculated data in the correct position
    thread_ids_data[nthreads][chunk_idx] = [
        round(time, 3),                  # Raw time
        round(serial_time / time, 1),   # Example calculation 1
        round(serial_time / (time * (nthreads+2)), 2),  # Example calculation 2
    ]
    return thread_ids_data


if __name__ == "__main__":
    # Example configuration
    levels = [1, 2, 3]
    chunks = [10, 20, 40]
    thread_range = range(2, 13)  # Threads from 2 to 12

    for level in levels:
        os.system(f"make clean")
        os.system(f"make LEVEL={level}")

        # Initialize the thread_ids_data: A 2D list [nthreads][chunks]
        thread_ids_data = [[None for _ in chunks] for _ in range(len(thread_range))]

        for chunk_idx, chunk in enumerate(chunks):
            for thread_idx, nthreads in enumerate(thread_range):
                try:
                    # Use subprocess to run the program and capture the output
                    result = sp.check_output(['build/ask2_p', str(nthreads), str(chunk)])
                    thread_ids_data = extract_thread_ids(thread_ids_data, result, thread_idx, chunk_idx)
                except Exception as e:
                    print(f"Error running subprocess for nthreads={nthreads}, chunk={chunk}: {e}")
                    continue

        # Generate and print the LaTeX table for this level
        latex_table = generate_latex_table(thread_ids_data, level, chunks, thread_range)
        print(latex_table)
        print("%\n")
