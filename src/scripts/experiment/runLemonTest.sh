#!/bin/bash

thisDir="$(dirname $0)"
thisDir="$(readlink -f "$thisDir")"

lemonProgramDir="$thisDir"/../../bin/main/centrality/tests

pushd "$lemonProgramDir"

if [ "$#" -ne 1 ]; then
  echo "Usage: runLemonTest <input>"
else
./centrality_test "$1"
fi

popd
