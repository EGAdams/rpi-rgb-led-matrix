# for each folder in the source directory, find the folder with the same name in the 
# destination directory overwrite the contents of the folder in the destination 
# directory with the content of the folders from the source directory
# place a space after the open parenthesis of non-empty function parameters
# place a space before the closing parenthesis of non-empty function parameters
import os
source_directory      = "C:\Users\EG\Desktop\2022\may\2nd_week\pickle_cpp"
destination_directory = "C:\Users\EG\Desktop\2022\june\3rd_week\pickleball_library"

for folder in os.listdir(source_directory):
    if os.path.isdir(os.path.join(source_directory, folder)):
        for file in os.listdir(os.path.join(source_directory, folder)):
            if file[-2:] == ".h":
                with open(os.path.join(source_directory, folder, file), "r") as source:
                    with open(os.path.join(destination_directory, folder, file), "w") as destination:
                        for line in source:
                            if line[0] == "#": # preprocessor directives are not modified
                                destination.write(line)

                            elif line[0] == "/" and line[1] == "/": # comments are not modified
                                destination.write(line)

                            elif line[0] == "/" and line[1] == "*": # comments are not modified
                                destination.write(line)

                            elif "(" in line and ")" in line: # function parameters are modified to have a space after the opening parenthesis and before the closing parenthesis (if they exist) 
                                index = 0

                                while index < len(line): # find the first opening parenthesis that is not inside a string literal or comment (// or /* */) or preprocessor directive (#define ...) 

                                    if index > 0 and (line[index - 1] == '"' or (line[index - 1] == '/' and line[index - 2] != '/') or (line[index - 1] == '*' and line[index - 2] != '/') or (line[index - 1] == '#')):  # ignore opening parenthesis inside string literals, comments, preprocessor directives 

                                        index += 1

                                    elif index > 0 and (line[index - 1].isalpha() or line[index - 1].isdigit()): # ignore opening parenthesis that is part of an identifier name 

                                        index += 1

                                    else: # found an opening parenthesis that is not inside a string literal or comment (// or /* */) or preprocessor directive (#define ...), so add a space after it 

                                        break;

                                while index < len(line): # find the first closing parenthesis that is not inside a string literal or comment (// or /* */) or preprocessor directive (#define ...) 

                                    if index > 0 and (line[index - 1].isalpha() or line[index - 1].isdigit()): # ignore closing parenthesis that is part of an identifier name 

                                        index += 1

                                    else: # found a closing parenthesis that is not inside a string literal or comment (// or /* */) or preprocessor directive (#define ...), so add a space before it 

                                        break;  

                        #destination.write(line[:index] + " " + line[index:])

                            else: # all other lines are not modified
                                destination.write(line)