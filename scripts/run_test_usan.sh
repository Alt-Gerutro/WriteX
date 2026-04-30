#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** USan (undefined behavior) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=USan -DCMAKE_CXX_COMPILER=clang++
cmake --build build

cd build
ctest --output-on-failure