#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** ASan (address) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=ASan
cmake --build build

cd build
ctest --output-on-failure