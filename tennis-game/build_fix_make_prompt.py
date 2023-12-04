# tool to create a prompt for debugging
import subprocess
import sys
import os
import re
import pyperclip

class CommandRunner:
    def __init__(self, command, args=[]):
        self.command, self.args = self.parse_command(command)
        # If additional args are provided, append them
        self.args.extend(args)

    @staticmethod # in case we want to run a command without creating an instance of the class
    def parse_command(command_string):
        parts = command_string.split()  # Split the command string into a list of words
        command = parts[0]              # The first part is the command
        args = parts[1:]                # The rest are the arguments
        return command, args

    def run(self):
        try:
            command_list = [self.command] + self.args  # Combine command and arguments
            print("running command:", " ".join(command_list))
            output = subprocess.check_output(command_list, stderr=subprocess.STDOUT, text=True)
            return output
        except subprocess.CalledProcessError as e:
            return e.output
        
# get the source from the make file into a variable. /home/adamsl/rpi-rgb-led-matrix/tennis-game/Makefile is the location of the make file
make_file_text = open( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/Makefile", 'r').read()
main_class = input ( "Enter the source directory: " )

class_name = '' # list the files in the current directory and get the class name from the .h file
os.chdir( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/" ) # change directory

# print the current directory
print( "current directory: " )
print( os.getcwd() )

command_to_run = input( "Please enter the command to run for this debug session: " )

command_to_run = CommandRunner( command_to_run )
command_output = command_to_run.run()

# files = os.listdir(  )
# for file in files:
#     if re.search('.h', file):
#         class_name = file.split('.')[0]
#         break

header_file_text = open( main_class + "/" + main_class + '.h'  , 'r').read()
cpp_file_text    = open( main_class + "/" + main_class + '.cpp', 'r').read()

# start building the prompt
prompt = """
# Persona
- World-class C++ developer
- Expert at debugging g++ compiler and linker errors

# Goal
- Debug the errors in the ouput of make.

# Header file for the class: """ + class_name + """
```cpp
""" + header_file_text + """
```

# Cpp file for the class: """ + class_name + """
```cpp
""" + cpp_file_text + """
```

# Makefile
```make
""" + make_file_text + """
```

# Command output
```
""" + command_output + """
```
"""

# open the .cpp file and read the list of #include statements
# make a regex to pull all of the paths from the #include statements
# loop through each line of the .cpp file and pull out the #include statements
# print the contents of the header file

# open .h and read it
header_file = open(  main_class + "/" + main_class + '.h', 'r')
header_file_text = header_file.read()
print ( header_file_text )


header_files = []
for line in header_file_text.split('\n'):
    if re.search('#include', line):
        print(line)
        if re.search('json.h', line):
            print( "skipping json.h" )
            continue
        regex = re.compile('#include "(.*)"') # pull the include path
        match = regex.search(line)
        if match: # if there is a match add it to the array of header files
            print( "appending: " + match.group(1))
            header_files.append(match.group(1))
            
prompt += """
# Header files for the class: """ + class_name + """\n
""" # loop through the header files and add them to the prompt
print ( "entering header files loop..." )
for header_file in header_files:
    print( header_file )
    # open the each header file and add it to the prompt
    added_header_file = open( main_class + "/" + header_file, 'r')
    header_file_text = added_header_file.read()
    prompt += """
    \n### Header file: """ + header_file + """    
```cpp
""" + header_file_text + """
```\n
"""

prompt_file = open('fix_make.md', 'w')  # open the file to write to
prompt_file.write( prompt )             # write the prompt to a file
pyperclip.copy(prompt)                  # put the prompt on the clipboard
