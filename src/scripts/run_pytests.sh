#!/bin/bash -eu
thisDir=$(dirname "$0")
thisDir=$(readlink -f "$thisDir")

dir=${1:-}

if [ "" == "$dir" ]; then
  tmpDir=$(mktemp -d)
else
  tmpDir="$dir"
  mkdir "$tmpDir"
fi

export tmpDir="$tmpDir"

export PYTHONPATH=:"${PYTHONPATH:-}"

pushd "$thisDir"
  py.test ${pytest_params:-}
popd

if [ "" == "$dir" ]; then
  rm -rf "$tmpDir"
fi

exit

