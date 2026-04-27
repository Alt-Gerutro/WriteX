#!/bin/bash

cd "$(dirname "$0")" || exit
./build_examples.sh
cd ../build || exit

find . -maxdepth 1 -type f -name "example_*" -exec sh -c 'echo "Running: {}" | sed "s/.\///"' \; -exec ./{} \;
