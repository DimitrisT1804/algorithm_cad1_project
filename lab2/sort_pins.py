def sort_lines(line):
    try:
        n_pin = int(line.split()[1][1:])
    except ValueError:
        # Handle the case where the second element is not a valid integer
        n_pin = float('inf')  # Assign a large value to non-numeric pins
    return n_pin


# Input and output file paths
input_file_path = 'text1.txt'  # Replace with the actual path of your input file
output_file_path = 'output.txt'  # Replace with the desired path for the output file

# Read input from the file
with open(input_file_path, 'r') as file:
    lines = file.readlines()

# Sort the lines based on the N pin
sorted_lines = sorted(lines, key=sort_lines)

# Write the sorted lines to the output file
with open(output_file_path, 'w') as file:
    file.writelines(sorted_lines)

print(f"Sorting completed. Output written to {output_file_path}")
