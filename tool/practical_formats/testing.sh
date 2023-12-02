#!/bin/bash

# List of arguments for hw2
arguments=("c17.practicalformat" "c432.practicalformat" "c499.practicalformat" 
"c880.practicalformat" "c1355.practicalformat" "c1908.practicalformat" "c3540.practcalformat"
"c5315.practicalformat" "c6288.practicalformat" "c7552.practicalformat" "lpffir.practicalformat"
"PID.practicalformat")

for arg in "${arguments[@]}"; do
    # Run hw2 with the current argument using valgrind
    echo -e "--------- Input file is $arg ---------\n"
    valgrind_output=$(valgrind --leak-check=full --show-leak-kinds=all ../hw2 "$arg" 2>&1)

    # Capture the IO pin number from the output
    io_pin=$(echo "$valgrind_output" | grep -oP 'IO pins are \K\d+')

    comp_num=$(echo "$valgrind_output" | grep -oP 'comps are \K\d+')

    # Check the exit status
    exit_status=$?

    if [ $exit_status -eq 139 ]; then
        # Segmentation fault occurred, exit the loop
        echo "Segmentation fault occurred with argument: $arg. Exiting."
        break
    fi

    # Print the IO pin number for the current iteration
    echo "IO pin number for $arg: $io_pin"
    echo -e "comps number for $arg: $comp_num\n"

    # Sleep for a while before the next iteration
    sleep 1
done
