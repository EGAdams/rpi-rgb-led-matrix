# pass the name of the python file

import subprocess

code_to_debug = "checkLibraryInstallation.py"

# open the source code file and read the contents
# source_code = open( sys.argv[1], 'r' ).read()
source_code = open( code_to_debug, 'r' ).read()

# run the code to debug to get the error message
error_message = subprocess.check_output( [ 'python', code_to_debug ] )



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
```bash {error_message}```
"""
)
    