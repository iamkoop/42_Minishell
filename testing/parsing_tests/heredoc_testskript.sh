#!/bin/bash
# Simple manual test script

# In Order to check that you have to comment out unlink in te heredoc function
# Test: Check if file was created
if [ -f .hd/File_00.txt ]; then
    echo "✓ File created successfully"
    echo "Content:"
    cat .hd/File_00.txt
else
    echo "✗ File not created"
fi

# Cleanup
rm .hd/File_00.txt
