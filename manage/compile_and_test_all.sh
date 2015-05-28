#!/bin/bash -eu
thisDir=$(dirname $0) || false

rm -rf "$thisDir"/../src/bin/

pushd "$thisDir"/../src/cpp
  scons -Q test=True
popd

"$thisDir"/../src/cpp/run_tests.sh
