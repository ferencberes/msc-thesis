#!/bin/bash -eu

thisDir=$(dirname $0) || false
export LD_LIBRARY_PATH="$thisDir"/../dep/log4cpp/lib/

for i in $(find $thisDir/../bin/test/*/* -type f | grep -v \.o$); do
  echo $i;
  $i;
  echo;
done

exit

