#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** MSan (memory) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=MSan -DCMAKE_CXX_COMPILER=clang++
cmake --build build

cd build
ctest --output-on-failure