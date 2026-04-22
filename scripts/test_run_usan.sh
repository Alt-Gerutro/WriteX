#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=USan
cmake --build build

cd build
ctest --output-on-failure