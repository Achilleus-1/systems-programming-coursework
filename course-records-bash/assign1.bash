#!/bin/bash
echo "Enter one of the following actions or press CTRL-D to exit.
C - create a new course record
R - read an existing course record
U - update an existing course record
D - delete an existing course record
E - update enrolled student count of existing course
T - show total course count
"
read -p "Enter your choice: " action


case "${action^^}" in
    C) ./create.bash ;;
    R) ./read.bash;;
    D) ./delete.bash ;;
    E) ./enroll.bash ;;
    T) ./total.bash ;;
    U) ./update.bash ;;
    *) echo "Invalid option." ;;
esac