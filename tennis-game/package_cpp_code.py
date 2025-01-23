#
# last day of september in this nice largo library upstairs
#
import os, re

import re

def extract_and_write_method(input_file, method_name, output_file):
    """
    Read a C++ file, extract a specific method, and write it to an output file with code fences.

    :param input_file: The path to the input C++ file
    :param method_name: The name of the method to extract
    :param output_file: The file to write the extracted method with code fences
    """
    try:
        with open(input_file, 'r') as f:
            file_content = f.read()

        # Updated regular expression to match the method definition with parameters and body
        method_pattern = re.compile(
            rf"void\s+{re.escape(method_name)}\s*\(.*?\)\s*\{{.*?\n\}}", re.DOTALL
        )

        match = method_pattern.search(file_content)
        if match:
            method_body = match.group(0)
            with open(output_file, 'w') as f:
                f.write("```cpp\n")
                f.write(method_body)
                f.write("\n```\n")
            print(f"Method '{method_name}' successfully extracted to {output_file}.")
        else:
            print(f"Method '{method_name}' not found in the provided content.")
    except FileNotFoundError:
        print(f"File '{input_file}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
input_cpp_file = "tennis-game.cpp"  # Path to the input C++ file
extract_and_write_method(input_cpp_file, "run_remote_listener", "run_remote_listener.cpp")


# Example usage
input_cpp_file = "tennis-game.cpp"  # Path to the input C++ file
extract_and_write_method(input_cpp_file, "run_remote_listener", "run_remote_listener.cpp")


def extract_cpp_class_or_struct(content):
    """
    Extracts the first C++ class or struct from the given content, ensuring the full declaration line is included.
    """
    depth = 0
    start_index = None
    class_or_struct_started = False
    for index, char in enumerate(content):
        if char == '{' and not class_or_struct_started:
            depth += 1
            if depth == 1:
                # Find the start of the class or struct declaration
                declaration_start = content.rfind('\n', 0, start_index) if start_index else 0
                return content[declaration_start:index+1] + extract_cpp_class_or_struct(content[index+1:])
        elif char == '{':
            depth += 1
        elif char == '}':
            depth -= 1
            if depth == 0:
                # End of class or struct
                return content[start_index:index+1]
        elif char == '\n' and depth == 0:
            # Possible start of a new class or struct declaration
            start_index = index
            class_or_struct_started = False
        else:
            # Check if we are at the start of the class or struct declaration
            if depth == 0 and not class_or_struct_started and (content[index:index+5] == 'class' or content[index:index+6] == 'struct'):
                start_index = index
                class_or_struct_started = True
    return ""

def process_cpp_file(input_path, output_file_path, append_mode=False):
    """
    Processes a single C++ file to extract classes or structs and writes them to the output file.
    """
    with open(input_path, 'r') as cpp_file:
        content = cpp_file.read()
        cpp_code = extract_cpp_class_or_struct(content)
        if cpp_code:
            with open(output_file_path, 'a' if append_mode else 'w') as output_file:
                output_file.write(f"```cpp\n{cpp_code}\n```\n\n")

def process_cpp_files_in_directory(input_dir, output_file_path, append_mode_arg=False):
    """
    Processes all C++ files in the given directory to extract classes or structs and append them to the output file.
    """
    for file_name in os.listdir(input_dir):
        if file_name.endswith(".cpp") or file_name.endswith(".h"):
            full_path = os.path.join(input_dir, file_name)
            process_cpp_file(full_path, output_file_path, append_mode_arg )


# Path setup 
base_dir = "/home/adamsl/rpi-rgb-led-matrix/tennis-game"

# output prompt file
output_file_path = os.path.join(base_dir, "packaged_code.md")

path = os.path.join( base_dir, "tennis-game.cpp" )
extract_and_write_method( path, "run_remote_listener", output_file_path )

path = os.path.join( base_dir, "RemotePairingScreen/RemotePairingScreen.cpp" )
process_cpp_file( path, output_file_path, True )

path = os.path.join( base_dir, "KeyboardInputWithTimerWithTimerWithTimerWithTimer/KeyboardInput.cpp" )
process_cpp_file( path, output_file_path, True )

path = os.path.join( base_dir, "Blinker/Blinker.h" )
process_cpp_file( path, output_file_path, True )
