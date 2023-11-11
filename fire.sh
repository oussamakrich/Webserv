#!/bin/bash
INT=1
while true; do
    # Run the command and store the exit code
    curl -X POST "file1=@/Users/moulmoud/goinfre/test.mkv" http://10.13.3.5:8080
    exit_code=$?

    # Check if the exit code is non-zero (indicating an error)
    if [ $exit_code -ne 0 ]; then
        echo "Command exited with error code $exit_code. Retrying..."
		break
    fi
	INT=$(( $INT + 1 ))
	echo $INT
    # Add a delay before retrying (optional)

done
