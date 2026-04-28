#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=MSan
cmake --build build

cd build

echo "***** MSan (memory) *****"
ctest --output-on-failure