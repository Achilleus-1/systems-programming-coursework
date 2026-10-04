#!/bin/bash

# reads everyhting
echo "Enter department code (2-3 letters):"
read -r dept_code
echo "Enter department name:"
read -r dept_name
echo "Enter course number (4 digits):"
read -r course_num
echo "Enter course name:"
read -r course_name
echo "Enter course schedule (MWF or TH):"
read -r course_sched
echo "Enter course start date (MM/DD/YY):"
read -r course_start
echo "Enter course end date (MM/DD/YY):"
read -r course_end
echo "Enter course credit hours (integer):"
read -r course_hours
echo "Enter initial enrollment (integer):"
read -r course_size

# Caps lock
course_file="./data/${dept_code^^}${course_num}.crs"

# see if course file already exists
if [[ -f "$course_file" ]]; then
    echo "ERROR: course already exists"
    exit 1
fi

# Create course file
{
    echo "$dept_code $dept_name"
    echo "$course_name"
    echo "$course_sched $course_start $course_end"
    echo "$course_hours"
    echo "$course_size"
} > "$course_file"

# Log creation
log_file="./data/queries.log"
echo "$(date) CREATED: $dept_code $course_num $course_name" >> "$log_file"

./assign1.bash