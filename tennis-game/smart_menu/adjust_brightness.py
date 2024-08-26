#
# 081524
#
def main():
    # Open and read the current brightness value from the file
    try:
        with open("brightness.txt", "r") as file:
            current_brightness = file.read().strip()
    except FileNotFoundError:
        print("brightness.txt not found. Creating a new one.")
        current_brightness = "0"  # default value if file does not exist

    # Display the current brightness level
    print(f"Brightness is set at {current_brightness}. Enter new value:")

    # Get the new brightness value from the user
    new_brightness = input()

    # Write the new brightness value to the file
    with open("brightness.txt", "w") as file:
        file.write(new_brightness)

    print(f"Brightness level updated to {new_brightness}.")

if __name__ == "__main__":
    main()
