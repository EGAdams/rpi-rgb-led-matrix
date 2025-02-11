import subprocess

# Get the list of modified and deleted files using relative paths
result = subprocess.run(["git", "status", "--porcelain"], capture_output=True, text=True)
files_to_restore = []

# Define the prefix that needs to be removed
prefix = "tennis-game/"

for line in result.stdout.splitlines():
    parts = line.strip().split(maxsplit=1)  # Splitting into status and file
    if len(parts) == 2 and parts[0] in ("D", "M"):
        file_path = parts[1]
        # Remove the "tennis-game/" prefix if it exists
        if file_path.startswith(prefix):
            file_path = file_path[len(prefix):]
        files_to_restore.append(file_path)

# Restore all files if found
if files_to_restore:
    subprocess.run(["git", "restore", "--"] + files_to_restore, check=True)
    print("Restored the following files:")
    for file in files_to_restore:
        print(f" - {file}")
else:
    print("No modified or deleted files to restore.")
