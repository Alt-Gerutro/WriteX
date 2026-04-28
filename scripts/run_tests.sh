#/bin/bash

cd "$(dirname "$0")" || exit

./test_run_usan.sh
echo ""

./test_run_msan.sh
echo ""


./test_run_asan.sh
echo ""

./test_run_lsan.sh
echo ""

./test_run_tsan.sh
echo ""

echo "All sanitizers tested"