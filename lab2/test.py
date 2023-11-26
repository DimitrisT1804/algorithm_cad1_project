import re

def find_missing_connections(text1_file, text2_file):
    # Read text contents from files
    with open(text1_file, 'r') as file:
        text1 = file.read()

    with open(text2_file, 'r') as file:
        text2 = file.read()

    # Find connections based on the "CCs:" pattern
    connections_pattern = re.compile(r'CCs:(.*)', re.DOTALL)

    match1 = connections_pattern.search(text1)
    match2 = connections_pattern.search(text2)

    if match1 and match2:
        # Extract connections from the matched groups
        connections_text1 = match1.group(1).strip()
        connections_text2 = match2.group(1).strip()

        # Convert connections to lists
        connections_list1 = [int(x.strip()) for x in connections_text1.split(',')]
        connections_list2 = [int(x.strip()) for x in connections_text2.split(',')]

        # Identify missing connections
        missing_connections = set(connections_list1) - set(connections_list2)
        print(f"Missing connections in {text2_file}: {missing_connections}")
    else:
        print("Connections not found in the provided texts.")

# Example usage:
if __name__ == "__main__":
    # Get user input for text1 file and text2 file
    text1_file = input("Enter the path to text1 file: ")
    text2_file = input("Enter the path to text2 file: ")

    # Call the function with the provided inputs
    find_missing_connections(text1_file, text2_file)
