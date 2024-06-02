import os
BASE_PATH = "/home/adamsl/rpi-rgb-led-matrix/tennis-game/"

# Function to delete all log files
def find_directories_with_log():
    directories_with_log = []

    for root, dirs, files in os.walk( BASE_PATH ):
        if 'log.txt' in files:
            relative_path = os.path.relpath( root, BASE_PATH )
            directories_with_log.append( relative_path )

    return directories_with_log

# Define the list of objects with log.txt
def delete_all_logs():
    objects_with_logs = find_directories_with_log()
    for obj in objects_with_logs:
        log_file_path = os.path.join(obj, "log.txt")
        if os.path.exists(log_file_path):
            os.remove(log_file_path)
    print("Deleted all logs")
    objects_with_logs.clear()
    print()

# Run the script
if __name__ == "__main__":
    delete_all_logs()
