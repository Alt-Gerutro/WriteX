#!/bin/bash

cd "$(dirname "$0")" || exit
cd ../build || ./build_examples.sh

find . -maxdepth 1 -type f -name "example_*" -exec sh -c 'echo "Running: {}" | sed "s/.\///"' \; -exec ./{} \;
