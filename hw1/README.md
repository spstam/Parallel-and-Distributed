# Building and Running the Program

This program calculates matrix norms and generates plots with python script to visualize the results.

### Building the Program:

*	`make all`: Builds all targets (`norm1`, `norminf`,`build`).
*	`make norm1`: Creates `build/matrix_norm_1.bin`.
*	`make norminf`: Creates `build/matrix_norm_inf.bin`.
*	`make build`: Creates the `build` directory.

### Running the Program:

#### Using the command line

```
./build/<binary_name> <executions> <N>
```
where `<binary_name>` is either `matrix_norm_1.bin` or `matrix_norm_inf.bin`,
`executions` is the number of times we execute each version, and N the size of each matrix dimention.

#### Using the `plot.py` script

1.	`--norm`: Specifies the matrix norm to calculate (determines which C program is executed).
2.	`--n`:  Sets the size of the matrix (n x n).
3.	`--e`:  Controls the number of program executions for calculating average runtimes.
4.	`--rerun`:  Indicates whether to re-run the C program or use existing data for plotting.
>**Note:** The script includes error handling for cases where `--rerun` is not specified and no previous data is available.

*	`make plot_norm1`: Generates plot for norm-1, runs the program if there are no data.
	```bash
	python3 plot.py --norm=1 --n=32768 --e=20 --rerun 
	```

*	`make run_norm1`: Same, but always reruns the program.
	```bash
	python3 plot.py --norm=1 --n=32768 --e=20 --rerun 
	```

*	`make plot_norminf`: Generates plot for norm-inf, runs the program if there are no data.
	```bash
	python3 plot.py --norm=1 --n=32768 --e=20 
	```

*	`make run_norminf`: Same, but always reruns the program.
	```bash
	python3 plot.py --norm=inf --n=32768 --e=20 --rerun
	```

### Other Commands:

*  `make clean`: Removes all generated files