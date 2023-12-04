import os
import subprocess

def check_library_installation(library_name):
    try:
        result = subprocess.check_output(['ldconfig', '-p'], universal_newlines=True)
        return library_name in result
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        return False

# Usage:
print(check_library_installation('rgbmatrix'))
