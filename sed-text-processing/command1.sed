#!/bin/sed -f

# trailing spaces
s/[ \t]*$//

# Smush down whitespaces
/^[^\/]/s/[ \t]\+/ /g

#Binary sided spaces
# This regex looks for common binary operators and ensures spacing on either side.
s/\([^+<>=*/ \t]\)\([+*/<>-]\)\([^+<>=*/ \t]\)/\1 \2 \3/g
s/[ \t]*==[ \t]*/ == /g
s/[ \t]*<=[ \t]*/ <= /g
s/[ \t]*>=[ \t]*/ >= /g

#spaces
s/if[ \t]*( */if(/g
s/else if[ \t]*( */else if(/g