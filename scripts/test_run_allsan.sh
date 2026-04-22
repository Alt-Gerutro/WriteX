#/bin/bash

cd "$(dirname "$0")" || exit

echo "***** USan (undefined behavior) *****"
./test_run_usan.sh
echo ""

echo "***** MSan (memory) *****"
./test_run_msan.sh
echo ""

echo "***** ASan (address) *****"
./test_run_asan.sh
echo ""

echo "***** LSan (leak) *****"
./test_run_lsan.sh
echo ""

echo "***** TSan (thread) *****"
./test_run_tsan.sh
echo ""

echo "All sanitizers tested"