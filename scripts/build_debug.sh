#!/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build build