#!/bin/bash -eu
thisDir=$(dirname $0) || false

# delete former binary files
rm -rf "$thisDir"/../src/bin/

# compile c++ source files
pushd "$thisDir"/../src/cpp
  scons -Q test=True
popd

# run gtests for c++ code
"$thisDir"/../src/cpp/run_tests.sh

# run pytests for python scripts (for this pytest must be installed)
#"$thisDir"/../src/scripts/run_pytests.sh
