#!/usr/bin/env python3

import sys
import os
import csv
from collections import defaultdict




def process_files(filepaths):
    total_calls = 0
    first_call_line = {}
    last_call_line = {}
    problem_counts = defaultdict(int)
    division_counts = defaultdict(int)

    for filepath in filepaths:
        with open(filepath, newline='') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                if len(row) < 5:
                    continue
                
                call_id, timestamp, problem_type, address, division = row
                total_calls += 1
                call_date = timestamp.split(" ")[0]

                # Track first and last call per date
                if call_date not in first_call_line or timestamp < first_call_line[call_date][1]:
                    first_call_line[call_date] = row
                if call_date not in last_call_line or timestamp > last_call_line[call_date][1]:
                    last_call_line[call_date] = row

                # Count problems and divisions
                problem_counts[problem_type] += 1
                division_counts[division] += 1





    return total_calls, first_call_line, last_call_line, problem_counts, division_counts


def main():
    if len(sys.argv) != 2:
        print("Usage: ./assign4.py <data_directory_or_file>")
        sys.exit(1)

    input_path = sys.argv[1]
    filepaths = [os.path.join(input_path, f) for f in os.listdir(input_path) if f.endswith(".csv")] if os.path.isdir(input_path) else [input_path]

    total_calls, first_call_line, last_call_line, problem_counts, division_counts = process_files(filepaths)

    #output
    print("Total calls =", total_calls)
    for date in sorted(first_call_line):


        print(f"\nDate: {date}")
        print("Last call:", ", ".join(first_call_line[date])) 
        print("First call:", ", ".join(last_call_line[date]))  



    print("\nPer-problem totals:")
    for problem, count in problem_counts.items():
        if problem:
            print(f"{problem}: {count}")



    print("\nPer-division totals:")
    for division, count in division_counts.items():
        if division:
            print(f"{division}: {count}")

if __name__ == "__main__":
    main()
