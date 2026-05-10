#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** MSan (memory) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=MSan -DWRITEX_BUILD_TESTS=ON
cmake --build build --parallel $(nproc)

cd build
ctest --output-on-failure