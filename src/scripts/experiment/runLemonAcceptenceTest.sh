#!/bin/bash

thisDir="$(dirname $0)"
thisDir="$(readlink -f "$thisDir")"

echo "$thisDir" 

lemonProgramDir="$thisDir"/../../bin/main/centrality/tests
testInput="$thisDir"/../../../resources/example/digraph.lgf

pushd "$thisDir"

echo "This script tests the ListDigraphForCentrality class with its functions for a fixed input."
"$lemonProgramDir"/centrality_acceptence_test "$testInput"

popd

