#
# last day of september in this nice largo library upstairs
#
import os

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
project_dir = os.path.join(base_dir, "Airport_Project")
view_controllers_dir = os.path.join(project_dir, "view_controllers")
custom_views_dir = os.path.join(project_dir, "custom_views")
table_view_cell_dir = os.path.join(project_dir, "custom_views/table_view_cell")
factories_dir = os.path.join(project_dir, "factories")
keyboard_scroll_dir = os.path.join(project_dir, "keyboard_scroll")

# output prompt file
output_file_path = os.path.join(base_dir, "display_fix.md")

path = os.path.join( base_dir, "IDisplay" )
process_cpp_files_in_directory( path, output_file_path, False ) # create new file.  append is False

path = os.path.join( base_dir, "ScoreBoard" )
process_cpp_files_in_directory( path, output_file_path, True ) # append to file is True

