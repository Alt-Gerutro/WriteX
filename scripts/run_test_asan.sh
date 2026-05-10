#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** ASan (address) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=ASan -DWRITEX_BUILD_TESTS=ON
cmake --build build --parallel $(nproc) --clean-first

cd build
ctest --output-on-failure