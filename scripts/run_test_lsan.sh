#/bin/bash

cd "$(dirname "$0")" || exit
cd .. || exit

echo "***** LSan (leak) *****"
cmake . -B build -DCMAKE_BUILD_TYPE=LSan
cmake --build build

cd build
ctest --output-on-failure