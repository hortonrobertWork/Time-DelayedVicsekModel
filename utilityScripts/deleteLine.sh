#!/bin/bash

# Check if a folder path is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <folder_path>"
    exit 1
fi

folder_path="$1"

# Check if the folder exists
if [ ! -d "$folder_path" ]; then
    echo "Error: Folder '$folder_path' not found."
    exit 1
fi

# Process each file in the folder
for file_path in "$folder_path"/*; do
    # Check if the item is a file
    if [ -f "$file_path" ]; then
        # Use sed to delete the first line of the file
        sed -i '1d' "$file_path"
        echo "Deleted the first line in '$file_path'"
    fi
done

echo "Deletion process completed."
