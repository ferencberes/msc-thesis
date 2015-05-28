#!/bin/bash -eu

trap "echo Some errors occurred while installing gtest. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
DEP_DIR=$SCRIPT_DIR/../../dep/
INSTALL_DIR=$DEP_DIR/cmake/

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

tempdir=`mktemp -d`
trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://www.cmake.org/files/v3.2/cmake-3.2.1-Linux-x86_64.tar.gz

tar -zxvf cmake-3.2.1-Linux-x86_64.tar.gz

mv cmake-3.2.1-Linux-x86_64 $DEP_DIR/cmake

cd $INSTALL_DIR

CMAKE_DIR==`pwd`

echo "CMAKE was successsfully untarred in $CMAKE_DIR" 
