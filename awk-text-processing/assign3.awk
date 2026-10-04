#!/usr/bin/awk -f

BEGIN {
    FS = ","  # stop brekaing commas pls
    IGNORECASE = 1  # Just... INCASE

    total_calls = 0  

    split("", first_call_line)
    split("", last_call_line)
    split("", problem_counts)
    split("", division_counts)
}


{
    # troubleshooting worked
    gsub(/^"|"$/, "", $1)
    gsub(/^"|"$/, "", $2)
    gsub(/^"|"$/, "", $3)
    gsub(/^"|"$/, "", $4)
    gsub(/^"|"$/, "", $5)

    call_id = $1
    timestamp = $2
    problem_type = $3
    address = $4
    division = $5

    total_calls++

    # YYYY-MM-DD
    split(timestamp, datetime_parts, " ")
    call_date = datetime_parts[1]

    # irst and last call, but backwards
    if (!(call_date in first_call_line) || timestamp < first_call_line[call_date]) {
        first_call_line[call_date] = $0
    }
    if (!(call_date in last_call_line) || timestamp > last_call_line[call_date]) {
        last_call_line[call_date] = $0
    }

    # Accumulate problem type totals
    problem_counts[problem_type]++

    # Accumulate division totals
    division_counts[division]++
}

END {
    print "Total calls =", total_calls

    # I did it backwards so it can stay that way. if it works it works.
    for (date in first_call_line) {
        print "\nDate:", date
        print "Last call:", first_call_line[date]
        print "First call:", last_call_line[date]
    }

    print "\nPer-problem totals:"
    for (problem in problem_counts) {
        if (problem != "") {
            print problem ":", problem_counts[problem]
        }
    }

    print "\nPer-division totals:"
    for (division in division_counts) {
        if (division != "") {
            print division ":", division_counts[division]
        }
    }
}
