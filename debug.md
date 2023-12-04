
# Your role
- Expert Python developer

# Your task
- Given the source code and the error message, fix the error.

## Source code
```python import os
import subprocess

def check_library_installation(library_name):
    result = subprocess.run(['ldconfig', '-p'], capture_output=True, text=True)
    return library_name in result.stdout

# Usage:
print(check_library_installation('rgbmatrix'))

 ```

## Error message
```bash 
Traceback (most recent call last):
  File "/home/adamsl/rpi-rgb-led-matrix/linuxadmin/debug_rgb_lib_missing/checkLibraryInstallation.py", line 9, in <module>
    print(check_library_installation('rgbmatrix'))
  File "/home/adamsl/rpi-rgb-led-matrix/linuxadmin/debug_rgb_lib_missing/checkLibraryInstallation.py", line 5, in check_library_installation
    result = subprocess.run(['ldconfig', '-p'], capture_output=True, text=True)
AttributeError: 'module' object has no attribute 'run'

```
