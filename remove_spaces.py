import re

def remove_spaces_in_boolean_functions(input_file_path, output_file_path):
    with open(input_file_path, 'r') as file:
        lines = file.readlines()

    # Process lines to remove spaces in Boolean functions
    processed_lines = []
    for line in lines:
        if "Boolean Function:" in line:
            # Remove spaces only in the Boolean function line
            boolean_function = re.search(r'Boolean Function:\s*(.*)', line)
            if boolean_function:
                processed_line = line.replace(boolean_function.group(1), boolean_function.group(1).replace(' ', ''))
                processed_lines.append(processed_line)
            else:
                processed_lines.append(line)
        else:
            processed_lines.append(line)

    with open(output_file_path, 'w') as output_file:
        output_file.writelines(processed_lines)

# Example usage:
input_file_path = 'tool/practical_formats/three_counters.practicalformat'  # Replace with the actual file path
output_file_path = 'output_file.txt'  # Replace with the desired output file path

remove_spaces_in_boolean_functions(input_file_path, output_file_path)
