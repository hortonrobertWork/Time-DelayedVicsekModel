#!/bin/bash

# Set the directory where your files are located
directory="Slurm/"

# Set the string you want to search for and the string you want to replace it with
search_string="main1/"
replace_string="main1/src/"

# Use a for loop to iterate through all files in the directory
for file in "$directory"/*
do
    # Check if the file is a regular file
    if [ -f "$file" ]
    then
        # Use sed to replace the search string with the replace string in the file
        sed -i "s/$search_string/$replace_string/g" "$file"
        echo "Replaced in $file"
    fi
done
