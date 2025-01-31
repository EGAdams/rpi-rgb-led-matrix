#!/bin/bash

# Run make and capture output
output=$(make -f Makefile.remote_listener -k 2>&1)

# Print lines with errors and warnings
echo "$output" | grep -E '(.cpp:[0-9]+|error|warning)'

# Count errors
error_count=$(echo "$output" | grep -c "error")

# Print error count
echo "*** Make Error Count: $error_count ***"