#!/bin/bash -eu

trap "echo Some errors occurred while installing gtest. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
DEP_DIR=$SCRIPT_DIR/../../dep/
INSTALL_DIR=$DEP_DIR/gtest/

if [ -d "$INSTALL_DIR" ]; then
  echo "$INSTALL_DIR exists already. Exiting."
  exit 0
fi

tempdir=`mktemp -d`
trap "rm -rf $tempdir" EXIT

cd $tempdir

wget http://googletest.googlecode.com/files/gtest-1.6.0.zip

unzip gtest-1.6.0.zip
mv gtest-1.6.0 $DEP_DIR/gtest

cd $INSTALL_DIR

GTEST_DIR=`pwd`

mkdir build
cd build

g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o

echo "GTEST successsfully installed in $GTEST_DIR" 
