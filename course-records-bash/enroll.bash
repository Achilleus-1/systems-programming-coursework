#!/bin/bash

# Prompt for department code and course number
echo "Enter a department code and course number (e.g., cs 3423):"
read -r dept_code course_num

# Convert department code to uppercase
course_file="./data/${dept_code^^}${course_num}.crs"

# Check if course file exists
if [[ ! -f "$course_file" ]]; then
    echo "ERROR: course not found"
    exit 1
fi

# Prompt for enrollment change amount
echo "Enter an enrollment change amount (e.g., 3 for enroll or -2 for drop):"
read -r change_amt

# Read the course file, update the enrollment
{
    read -r dept_line
    read -r course_name
    read -r schedule_line
    read -r course_hours
    read -r course_size
} < "$course_file"

new_enrollment=$((course_size + change_amt))

{
    echo "$dept_line"
    echo "$course_name"
    echo "$schedule_line"
    echo "$course_hours"
    echo "$new_enrollment"
} > "$course_file"

# Logs change
log_file="./data/queries.log"
echo "$(date) ENROLLMENT: $dept_code $course_num $course_name changed by $change_amt" >> "$log_file"

./assign1.bash