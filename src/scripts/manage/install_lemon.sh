#!/bin/bash -eu

trap "echo Some errors occurred while installing lemon. Exiting." ERR

SCRIPT_DIR=$(readlink -f $(dirname "$0"))
DEP_DIR=$SCRIPT_DIR/../../dep
LEMON_SRC_DIR=$DEP_DIR/lemon_src
LEMON_BIN_DIR=$DEP_DIR/lemon_bin
CMAKE_BIN_DIR=$DEP_DIR/cmake/bin

if [ -d "$LEMON_SRC_DIR" ]; then
  echo "$LEMON_SRC_DIR exists already. Exiting."
  exit 0
fi

if [ -d "$LEMON_BIN_DIR" ]; then
  echo "$LEMON_BIN_DIR exists already. Exiting."
  exit 0
fi

tempdir=`mktemp -d`
trap "#rm -rf $tempdir" EXIT

cd $tempdir

wget http://lemon.cs.elte.hu/pub/sources/lemon-1.3.1.tar.gz

tar -zxvf lemon-1.3.1.tar.gz
mv lemon-1.3.1 "$LEMON_SRC_DIR" 

cd $LEMON_SRC_DIR
mkdir build
cd build
$CMAKE_BIN_DIR/cmake -DCMAKE_INSTALL_PREFIX="$LEMON_BIN_DIR" .. 
make
make install

cd $LEMON_BIN_DIR
LEMON_DIR=`pwd`

echo "LEMON successsfully installed in $LEMON_DIR" 
