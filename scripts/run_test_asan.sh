#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** ASan (address) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=ASan -DCMAKE_CXX_COMPILER=clang++
cmake --build build

cd build
ctest --output-on-failure