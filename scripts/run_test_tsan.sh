#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** TSan (thread) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=TSan -DCMAKE_CXX_COMPILER=clang++
cmake --build build

cd build
ctest --output-on-failure