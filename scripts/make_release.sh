#!/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

cd build
tar -czvf WriteX.tar.gz libwritex.so ../include ../LICENSE ../README.md