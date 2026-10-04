#!/bin/bash

# Part for changing values
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
echo "Enter course enrollment (integer):"
read -r course_size


course_file="./data/${dept_code^^}${course_num}.crs"


if [[ ! -f "$course_file" ]]; then
    echo "course doesnt exist"
    exit 1
fi


{
    read -r current_dept_line
    read -r current_course_name
    read -r current_schedule_line
    read -r current_course_hours
    read -r current_course_size
} < "$course_file"


current_course_sched=$(echo "$current_schedule_line" | cut -d' ' -f1)
current_course_start=$(echo "$current_schedule_line" | cut -d' ' -f2)
current_course_end=$(echo "$current_schedule_line" | cut -d' ' -f3)

# Makes sure it doesnt just leave a blank space when you press enter
dept_name=${dept_name:-$(echo "$current_dept_line" | cut -d' ' -f2-)}
course_name=${course_name:-$current_course_name}
course_sched=${course_sched:-$current_course_sched}
course_start=${course_start:-$current_course_start}
course_end=${course_end:-$current_course_end}
course_hours=${course_hours:-$current_course_hours}
course_size=${course_size:-$current_course_size}


{
    echo "$dept_code $dept_name"
    echo "$course_name"
    echo "$course_sched $course_start $course_end"
    echo "$course_hours"
    echo "$course_size"
} > "$course_file"

# Log the update
log_file="./data/queries.log"
echo "$(date) UPDATED: $dept_code $course_num $course_name" >> "$log_file"

echo "Course record updated successfully."

./assign1.bash