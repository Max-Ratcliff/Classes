#!/bin/bash

# task a.
sort roster.txt > roster_sortByLastName.txt

# task b.
sort roster.txt | awk '{print $1}' > roster_sortByLastName_lastName.txt

# task c.
sort -k 2 roster.txt > roster_sortByFirstName.txt

# task d.
sort -k 2 roster.txt | awk '{print $2}' > roster_sortByFirstName_firstName.txt

# task e.
sort -u roster.txt > roster_sortByLastName_noDuplicates.txt

# task f.
wc -l roster_sortByLastName_noDuplicates.txt | awk '{print $1}' 

# task g.
# prompted chatgpt: "explain in detail regex matching with awk"
# important pieces of LLM response:
# awk 'expression ~ /pattern/ {action}'
# expression: Can be a field (like $1, $2, etc.) or the entire line ($0).
# ~ /pattern/: Matches the field/line against the specified regular expression.
# {action}: Defines what happens when a match is found (like {print} to print matching lines).
# ^: Matches the beginning of a string. Example: ^a matches strings that start with "a".
# [abc]: Matches either "a", "b", or "c".

# attempt based on response:
awk '$2 ~ /^[LR]/ {print}' roster_sortByLastName_noDuplicates.txt > roster_LR.txt


# task h.
awk '$1 ~ /^[LR]/ {print $1}' roster_LR.txt | wc -l | awk '{print $1}' 
