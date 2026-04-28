#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=TSan
cmake --build build

cd build

echo "***** TSan (thread) *****"
ctest --output-on-failure