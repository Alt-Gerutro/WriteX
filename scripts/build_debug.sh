#!/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=Debug -DWRITEX_BUILD_EXAMPLES=ON -DWRITEX_BUILD_TESTS=ON
cmake --build build --parallel $(nproc)