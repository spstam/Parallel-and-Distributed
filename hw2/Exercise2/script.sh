#!/bin/bash

# Output file to store results
output_file="results.csv"
output_file1="results1.csv"
output_file2="results2.csv"

# Compile the C program
make clean
make LEVEL=1

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Initialize results
echo "nthreads,chunk,time" > $output_file
echo "nthreads,chunk,time" > $output_file1
echo "nthreads,chunk,time" > $output_file2

# Run the program for different threads and chunk sizes
for nthreads in {2..12}; do
    for chunk in 10 20 40; do
        # Run the program and capture the time
        result=$(./build/ask2_p $nthreads $chunk)
        echo "$nthreads,$chunk,$result" >> $output_file
    done
done

# Compile the C program
make clean
make LEVEL=2

for nthreads in {2..12}; do
    for chunk in 10 20 40; do
        # Run the program and capture the time
        result=$(./build/ask2_p $nthreads $chunk)
        echo "$nthreads,$chunk,$result" >> $output_file1
    done
done

# Compile the C program
make clean
make LEVEL=3

for nthreads in {2..12}; do
    for chunk in 10 20 40; do
        # Run the program and capture the time
        result=$(./build/ask2_p $nthreads $chunk)
        echo "$nthreads,$chunk,$result" >> $output_file2
    done
done

echo "Execution complete. Results saved."
