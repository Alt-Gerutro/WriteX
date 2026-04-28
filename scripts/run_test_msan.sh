#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** MSan (memory) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=MSan
cmake --build build

cd build
ctest --output-on-failure