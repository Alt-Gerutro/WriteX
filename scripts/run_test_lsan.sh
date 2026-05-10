#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** LSan (leak) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=LSan -DWRITEX_BUILD_TESTS=ON
cmake --build build --parallel $(nproc)

cd build
ctest --output-on-failure