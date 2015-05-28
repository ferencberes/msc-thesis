#!/bin/bash -eu
thisDir="$(dirname $0)"
thisDir="$(readlink -f "$thisDir")"

# delete all formerly installed dependency
rm -rf "$thisDir"/../src/dep
# this scripts install gtest and lemonc++ dependencies into src/dep
"$thisDir"/../src/scripts/manage/install_dependencies.sh

echo "install done"

