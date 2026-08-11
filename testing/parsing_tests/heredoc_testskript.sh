#!/bin/bash
# Simple manual test script

# In Order to check that you have to comment out unlink in te heredoc function
# Test: Check if file was created

counter=0
found_any=false

while true; do
    filename=$(printf "File_%02d.txt" $counter)
    filepath=".hd/$filename"
    
    if [ -f "$filepath" ]; then
        echo "✓ File created successfully: $filename"
        echo "Content:"
        cat "$filepath"
        echo ""
        found_any=true
        ((counter++))
    else
        break
    fi
done

if [ "$found_any" = false ]; then
    echo "✗ No heredoc files found"
fi

counter=0
while true; do
    filename=$(printf "File_%02d.txt" $counter)
    filepath=".hd/$filename"
    
    if [ -f "$filepath" ]; then
        rm "$filepath"
        echo "Deleted $filename"
        ((counter++))
    else
        break
    fi
done 
