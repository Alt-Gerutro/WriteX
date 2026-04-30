#!/bin/bash

echo "Changind directory..."
cd "$(dirname "$0")" || exit
cd .. || exit
echo "Now in $(pwd)"

ver=$(<VERSION)
archive_name="WriteX-v$ver"
archive_files=("libwritex.so" "../include" "../LICENSE" "../README.md")
echo "Version name: $ver"
echo "Archive name: $archive_name"
echo "Archive content: ${archive_files[@]}"

echo "Building project as Release..."
cmake . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
cmake --build build
echo "Project was built"

echo "Removing old archives..."
rm -f *.tar.gz
rm -f *.zip
echo "Old archives removed"

echo "Archiving project..."
cd build
tar -czvf $archive_name.tar.gz ${archive_files[@]}
7z a $archive_name.zip ${archive_files[@]}
echo "Project was archived"

echo "Publishing to GitHub (draft)"
gh release create "v$ver" -d --title "v$ver" --notes "$(<"../release_notes.md")" $archive_name.tar.gz $archive_name.zip