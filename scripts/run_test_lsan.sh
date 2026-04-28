#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

cmake . -B build -DCMAKE_BUILD_TYPE=LSan
cmake --build build

cd build

echo "***** LSan (leak) *****"
ctest --output-on-failure