#!/bin/bash

# asks code and course number
echo "Enter a department code and course number (e.g., cs 3423):"
read -r dept_code course_num

# Caps lock basically
course_file="./data/${dept_code^^}${course_num}.crs"

# Check if course file exists
if [[ ! -f "$course_file" ]]; then
    echo "ERROR: course not found"
    exit 1
fi


{
    read -r dept_line
    read -r course_name
    read -r schedule_line
    read -r course_hours
    read -r course_size
} < "$course_file"

# shows course details
echo "Course department: $dept_line"
echo "Course number: $course_num"
echo "Course name: $course_name"
echo "Scheduled days: ${schedule_line%% *}"
echo "Course start: $(echo $schedule_line | cut -d' ' -f2)"
echo "Course end: $(echo $schedule_line | cut -d' ' -f3)"
echo "Credit hours: $course_hours"
echo "Enrolled Students: $course_size"

./assign1.bash