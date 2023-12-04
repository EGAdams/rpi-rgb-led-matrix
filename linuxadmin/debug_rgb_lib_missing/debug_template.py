# pass the name of the python file

import subprocess

code_to_debug = "/home/adamsl/rpi-rgb-led-matrix/linuxadmin/debug_rgb_lib_missing/checkLibraryInstallation.py"

# open the source code file and read the contents
# source_code = open( sys.argv[1], 'r' ).read()
source_code = open( code_to_debug, 'r' ).read()

def run_subprocess(command):
    try:
        # Run the command and get the output
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)
        return result.stdout, result.stderr
    except Exception as e:
        print(f"An error occurred: {e}")
        return None, None
# run the code to debug to get the error message
# error_message = subprocess.check_output( [ 'python', code_to_debug ] )
stdout, stderr = run_subprocess( f"python {code_to_debug}" )



# write the next string containing the source code and the error message to
# a file called debug.md
with open( 'debug.md', 'w' ) as f:
    f.write( f"""
# Your role
- Expert Python developer

# Your task
- Given the source code and the error message, fix the error.

## Source code
```python {source_code} ```

## Error message
```bash 
{stderr}
```
"""
)


def run_subprocess(command):
    try:
        # Run the command and get the output
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)
        return result.stdout, result.stderr
    except Exception as e:
        print(f"An error occurred: {e}")
        return None, None

# Example usage:
# command = "ls"  # Replace 'ls' with the command you want to run
# stdout, stderr = run_subprocess(command)
# if stdout:
#     print(f"Output: {stdout}")
# if stderr:
#     print(f"Error: {stderr}")
    