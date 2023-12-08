#!/bin/tcl

# Get the absolute path to the executable
# set executable_path [file normalize ./cad_tool]

# List of arguments for hw2
set arguments {"c17.practicalformat" "c432.practicalformat" "c499.practicalformat"  "c1355.practicalformat" "c1908.practicalformat" "c3540.practcalformat"
"c5315.practicalformat" "c6288.practicalformat" "c7552.practicalformat" "lpffir.practicalformat"
"PID.practicalformat"}

foreach arg $arguments {
    # Run cad_tool with the current argument using valgrind
    puts "--------- Input file is $arg ---------"
    # set valgrind_output [exec read_design $arg 2>@1]
    read_design practical_formats/$arg

    # if {$exit_status == 139} {
    #     # Segmentation fault occurred, exit the loop
    #     puts "Segmentation fault occurred with argument: $arg. Exiting."
    #     break
    # }

    # Print the IO pin number for the current iteration
    after 1000
    puts "Complete test $arg"
    clear_design
    update
    
    # Sleep for a while before the next iteration
    after 1000
}
