import subprocess
import matplotlib.pyplot as plt

n_values = [120, 240, 360, 480, 600, 1200, 1800, 2400, 3000]
np_values = [2, 3, 4, 5, 6, 8, 10, 12]

with open("results.txt", "w") as f:
    f.write("np/n\t")
    for n in n_values:
        f.write(f"{n}\t")
    f.write("\n")

    # Loop over the np values
    for np in np_values:
        print(np)
        f.write(f"{np}\t")
        # Loop over the n values
        for n in n_values:
            print(n)
            # Run the program and capture the output
            command = f"mpirun -np {np} --oversubscribe ./ask3"
            process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
            process.stdin.write(f"{n}\n".encode())
            process.stdin.flush()
            output, error = process.communicate()

            # Extract the execution time from the output
            execution_time = float(output.decode().strip())

            # Write the execution time to the file
            f.write(f"{execution_time:.6f}\t")
        f.write("\n")

# Read the results from the file
results = {}
with open("results.txt", "r") as f:
    lines = f.readlines()
    header = lines[0].strip().split("\t")
    for line in lines[1:]:
        row = line.strip().split("\t")
        np = int(row[0])
        results[np] = [float(x) for x in row[1:]]

# Create the table
print("Table 3. Execution times for different n and np")
print("np/n\t" + "\t".join(str(n) for n in n_values))
for np, times in results.items():
    print(f"{np}\t" + "\t".join(f"{t:.6f}" for t in times))

# Create the plot
fig, ax = plt.subplots()
for np, times in results.items():
    ax.plot(n_values, times, label=f"np={np}")
ax.set_xlabel("n")
ax.set_ylabel("Parallel time (sec)")
ax.set_title("Parallel Time vs # of nodes")
ax.legend()
plt.savefig("plot.svg")
