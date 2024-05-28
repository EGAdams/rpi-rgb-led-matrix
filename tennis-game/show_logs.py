import pexpect
import json
import os
from datetime import datetime

# Define the list of objects with log.txt
objects_with_logs = [
    "GameState", "Drawer", "FontLoader", "GameModes", "GameObject", "GameWinSequence",
    "LogObjectFactory", "MatchWinSequence", "Mode1Functions", "Mode1Score",
    "Mode2Functions", "Reset", "ScoreBoard", "SetDrawer", "SetHistoryText",
    "SetLeds", "TennisConstants", "TieBreaker", "Undo", "WinSequences"
]

# Function to format and print log entries
def print_log_entries(log_entries):
    print( "\n\n" )
    print(f"{'Date':<8} {'Time':<10} {'Method':<12} {'Message':<20}")
    for entry in log_entries:
        timestamp = datetime.fromtimestamp(entry['timestamp'] / 1000.0)
        date_str = timestamp.strftime("%b %d")
        time_str = timestamp.strftime("%I:%M %p")
        method = entry['method']
        message = entry['message']
        print(f"{date_str:<8} {time_str:<10} {method:<12} {message:<20}")

# Function to show the main menu and handle user selection
def show_main_menu():
    while True:
        print("Show Logs for Object:")
        for idx, obj in enumerate(objects_with_logs, 1):
            print(f"{idx:02}. {obj}")
        print(f"{len(objects_with_logs) + 1:02}. Delete")
        print(f"{len(objects_with_logs) + 2:02}. Delete All")
        choice = input("Select an option: ")

        try:
            choice = int(choice)
            if 1 <= choice <= len(objects_with_logs):
                show_logs(objects_with_logs[choice - 1])
            elif choice == len(objects_with_logs) + 1:
                delete_log()
            elif choice == len(objects_with_logs) + 2:
                delete_all_logs()
            else:
                print("Invalid choice, please try again.")
        except ValueError:
            print("Invalid input, please enter a number.")

# Function to show logs for a specific object
def show_logs(object_name):
    log_file_path = os.path.join(object_name, "log.txt")
    if os.path.exists(log_file_path):
        with open(log_file_path, 'r') as log_file:
            log_entries = [json.loads(line) for line in log_file]
        print_log_entries(log_entries)
    else:
        print(f"No logs found for {object_name}")
    print()
    print( "\n\n")
    bitbucket = input( "<ENTER> to continue... " )

# Function to delete a specific log file
def delete_log():
    print("Delete Logs for Object:")
    for idx, obj in enumerate(objects_with_logs, 1):
        print(f"{idx:02}. {obj}")
    choice = input("Select an object to delete logs: ")

    try:
        choice = int(choice)
        if 1 <= choice <= len(objects_with_logs):
            object_name = objects_with_logs[choice - 1]
            log_file_path = os.path.join(object_name, "log.txt")
            if os.path.exists(log_file_path):
                os.remove(log_file_path)
                print(f"Deleted logs for {object_name}")
                objects_with_logs.remove(object_name)
            else:
                print(f"No logs found for {object_name}")
        else:
            print("Invalid choice, please try again.")
    except ValueError:
        print("Invalid input, please enter a number.")
    print()

# Function to delete all log files
def delete_all_logs():
    for obj in objects_with_logs:
        log_file_path = os.path.join(obj, "log.txt")
        if os.path.exists(log_file_path):
            os.remove(log_file_path)
    print("Deleted all logs")
    objects_with_logs.clear()
    print()

# Run the script
if __name__ == "__main__":
    show_main_menu()
