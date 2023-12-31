#!/bin/bash

# Call the Makefile to build the executable and generate the DOT file
make all

# Check if the make command was successful
if [ $? -eq 0 ]; then
    # Convert the DOT file to PNG
    dot -Tpng bdd.dot -o bdd.png
    echo "Conversion from DOT to PNG completed successfully."
else
    echo "Error: Makefile failed to build the executable or generate DOT file."
fi
