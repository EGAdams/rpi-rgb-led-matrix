# Create a menu in python that with be able to execute 3 other scripts also written in python.
# The menu should be able to exit the program.
# The menu should be able to go back to the main menu from the other scripts.
# The menu should be able to execute the other scripts more than once.
import os

def main():
    print("\n\n\n     //////////////////////////////" )
    print( "        Welcome to the main menu" )
    print("     //////////////////////////////\n" )
    print("     1. show digits")
    print("     2. git pull")
    print("     3. run the 1st digit script")
    print("     \n     4. Exit")

    choice = input("Please enter your choice: ")

    if choice == "1":
        print("openning the latest matrix project... ")
        # cd to the directory where the script is located
        os.chdir( "/home/dietpi/rpi-rgb-led-matrix" )
        # open vscode in the directory
        os.system("echo 'hello world'")
        main()
    elif choice == "2":
        print( "syncing with github... " )
        # open a child process to execute script 2
        os.system( "git pull" )
        main()
    elif choice == "3":
        default_options = '--led-multiplexing=2 --led-brightness=1 --led-chain=8 --led-pixel-mapper="U-mapper="U-mapper" --led-gpio-mapping=adafruit-hat --led-brightness=15'
        print( "running 1st digit script... " )
        os.system( 'sudo examples-api-use/text-example -f fonts/mspgothic_70_32 ' + default_options )
        main()

    elif choice == "4":
        print("Goodbye!")

    else:  # if the user enters anything other than 1, 2, 3 or 4 then the program will exit with an error message.

        print("Invalid input, please try again.")

    return 0


main()
