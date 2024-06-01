import os
import json
from datetime import datetime
BASE_PATH = "/home/adamsl/rpi-rgb-led-matrix/tennis-game/"

def find_directories_with_log():
    directories_with_log = []

    for root, dirs, files in os.walk( BASE_PATH ):
        if 'log.txt' in files:
            relative_path = os.path.relpath( root, BASE_PATH )
            directories_with_log.append( relative_path )

    return directories_with_log

# List of directories with log.txt files
directories_with_logs = find_directories_with_log()

# Function to format log entries
def format_log_entries(log_entries):
    formatted_entries = f"{'Date':<10} {'Time':<10} {'Method':<12} {'Message':<20}\n"
    for entry in log_entries:
        timestamp = datetime.fromtimestamp(entry['timestamp'] / 1000.0)
        date_str = timestamp.strftime("%b %d")
        time_str = timestamp.strftime("%I:%M %p")
        method = entry['method']
        message = entry['message']
        formatted_entries += f"{date_str:<10} {time_str:<10} {method:<12} {message:<20}\n"
    return formatted_entries

# Function to parse and output all logs
def parse_and_output_all_logs():
    for directory in directories_with_logs:
        log_file_path = os.path.join(BASE_PATH + directory, "log.txt")
        if os.path.exists(log_file_path):
            with open(log_file_path, 'r') as log_file:
                log_entries = [json.loads(line) for line in log_file]
            if log_entries:  # Only print if there are log entries
                print(f"{directory}\n{format_log_entries(log_entries)}")

# Run the function to parse and output all logs
if __name__ == "__main__":
    parse_and_output_all_logs()
