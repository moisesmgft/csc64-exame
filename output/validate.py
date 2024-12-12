import os

directory = 'output/'

# Get a list of all CSV files in the directory
csv_files = [f for f in os.listdir(directory) if f.endswith('.csv')]

if not csv_files:
    print("No CSV files found in the directory.")
    exit()

# Load the first CSV file as a reference
first_file_path = os.path.join(directory, csv_files[0])

with open(first_file_path, 'r') as f:
    reference_content = f.readlines()

for csv_file in csv_files[1:]:
    current_file_path = os.path.join(directory, csv_file)
    
    with open(current_file_path, 'r') as f:
        current_content = f.readlines()
    
    if reference_content != current_content:
        print(f"Files {csv_files[0]} and {csv_file} are not identical.")
        exit()


print("All CSV files are identical.")
