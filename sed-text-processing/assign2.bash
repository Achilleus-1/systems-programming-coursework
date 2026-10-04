#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: bash assign2.bash /path/to/input/file.c"
    exit 1
fi

input_file="$1"

if [ ! -f "$input_file" ]; then
    echo "The file doesnt exist silly"
    exit 1
fi

output_file="NEW_OUTPUT.c"


sed -f command1.sed "$input_file" > "$output_file"