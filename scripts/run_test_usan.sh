#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** USan (undefined behavior) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=USan -DWRITEX_BUILD_TESTS=ON
cmake --build build --parallel $(nproc)

cd build
ctest --output-on-failure