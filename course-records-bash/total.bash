#!/bin/bash

# totals .crs files in data folder
total=$(find ./data -name '*.crs' | wc -l)

echo "Total: $total"

./assign1.bash