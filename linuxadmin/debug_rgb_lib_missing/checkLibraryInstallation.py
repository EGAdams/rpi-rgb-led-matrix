import os
import subprocess

def check_library_installation(library_name):
    result = subprocess.run(['ldconfig', '-p'], capture_output=True, text=True)
    return library_name in result.stdout

# Usage:
print(check_library_installation('rgbmatrix'))

