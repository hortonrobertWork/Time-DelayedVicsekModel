#!/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 source_folder"
    exit 1
fi

source_folder="$1"
new_folder="data/combinedData/"

# Create the new folder if it doesn't exist
mkdir -p "$new_folder"

# Check if the source folder exists
if [ ! -d "$source_folder" ]; then
    echo "Error: Source folder does not exist."
    exit 1
fi

# Move files to the new folder with numbered filenames
counter=3
for file in "$source_folder"/*; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        extension="${filename##*.}"
        filename_noext="${filename%.*}"
        new_filename="${filename_noext}_${counter}.${extension}"
        mv "$file" "$new_folder/$new_filename"
    fi
done

echo "Files moved successfully to $new_folder."

