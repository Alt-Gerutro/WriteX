#/bin/bash

cd "$(dirname "$0")" || exit

./run_test_usan.sh
echo ""

./run_test_msan.sh
echo ""

./run_test_asan.sh
echo ""

./run_test_lsan.sh
echo ""

./run_test_tsan.sh
echo ""

echo "All sanitizers tested"