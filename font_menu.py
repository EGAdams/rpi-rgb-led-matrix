#
# Description: This script will create a menu of all the files in the fonts directory.
#
import os


def main():

    # list all files in the fonts directory.
    files = os.listdir("fonts")

    # create a menu selection for each file in the directory.
    print("\n")
    print("\n\n\n     //////////////////////////////" )
    print( "        Welcome to the fonts menu" )
    print("     //////////////////////////////\n" )
    print("     Select a font to use:\n\n")

    for i in range(len(files)):
        # only print files that contain mspgothic
        if "mspgothic" in files[i]:  # copilot suggestion
            print("     {}. {}".format(i+1, files[i]))

    # wait for user input and print the selected file.
    while True:
        try:
            choice = int(input("\n     Enter your choice: ")) - 1

            if choice < 0 or choice > len(files):
                raise ValueError

            #if choice == 0, exit the program.
            if choice == 0:
                print("     Goodbye!")
                return 0

            # run the text example with the chosen font.
            default_options = '--led-multiplexing=2 --led-chain=8 --led-pixel-mapper="U-mapper" --led-gpio-mapping=adafruit-hat --led-brightness=15'
            print("     showing digits with font {}... ".format(files[choice]))
            os.system( 'sudo examples-api-use/text-example -f fonts/' + files[ choice ] + ' ' + default_options )
            main()
            break

        except ValueError:
            print("\n     Invalid choice. Please try again.")

main()
