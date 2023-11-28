# tool to create a prompt to fix the make output
import subprocess
import sys
import os
import re

class CommandRunner:
    def __init__(self, command, args=[]):
        self.command = command
        self.args = args

    def run(self):
        try:
            output = subprocess.check_output([self.command] + self.args, stderr=subprocess.STDOUT, text=True)
            return output
        except subprocess.CalledProcessError as e:
            return e.output
        
# get the source from the make file into a variable. /home/adamsl/rpi-rgb-led-matrix/tennis-game/Makefile is the location of the make file
make_file_text = open( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/Makefile", 'r').read()

# human input source directory
main_class = input ( "Enter the source directory: " )


# list the files in the current directory and get the class name from the .h file
class_name = ''
# change directory to os.getcwd() + "/tennis-game/LogObjectContainer"
os.chdir( "/home/adamsl/rpi-rgb-led-matrix/tennis-game/" )

# print the current directory
print( "current directory: " )
print( os.getcwd() )

make_runner = CommandRunner( "make" )
make_output = make_runner.run()

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

# Make output
```
""" + make_output + """
```
"""

# open the .cpp file and read the list of #include statements
# make a regex to pull all of the paths from the #include statements
# loop through each line of the .cpp file and pull out the #include statements
# print the contents of the header file

# open LogObjectContainer.h and read it
header_file = open(  main_class + "/" + main_class + '.h', 'r')
header_file_text = header_file.read()
print ( header_file_text )


header_files = []
for line in header_file_text.split('\n'):
    if re.search('#include', line):
        print(line)
        # pull out the path to the header file
        # pull out the name of the header file
        regex = re.compile('#include "(.*)"')
        match = regex.search(line)
        # if there is a match add it to the array of header files
        if match:
            print( "appending: " + match.group(1))
            header_files.append(match.group(1))
            

# loop through the header files and add them to the prompt
prompt += """
# Header files for the class: """ + class_name + """\n
"""
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

# open the file to write to
prompt_file = open('fix_make.md', 'w')

# write the prompt to a file
prompt_file.write( prompt )
