#!/bin/bash

# for department code and course number
echo "Enter a department code and course number (e.g., cs 3423):"
read -r dept_code course_num

# Caps loick
course_file="./data/${dept_code^^}${course_num}.crs"

# Check if course file exists
if [[ ! -f "$course_file" ]]; then
    echo "ERROR: course not found"
    exit 1
fi

# Read the course name from the file before deleting
{
    read -r dept_line
    read -r course_name
} < "$course_file"

# Delete the course file
rm "$course_file"

# Log the deletion, ensuring it ends with course number and course name
log_file="./data/queries.log"
echo "$(date) DELETED: $course_num $course_name" >> "$log_file"

# Inform the user of successful deletion
echo "$course_num was successfully deleted."
