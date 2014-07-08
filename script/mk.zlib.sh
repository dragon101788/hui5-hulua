#!/bin/sh

cd libsrc/zlib-1.2.8
make clean
make CROSS_COMPILE=$CROSS_COMPILE
make install
make clean
