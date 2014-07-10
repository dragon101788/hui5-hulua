#!/bin/sh


cd libsrc/libpng-1.6.12
echo !!!!!!!!!${TOPDIR}!!!!!!!!!1
make distclean
./configure --host=arm-linux --prefix=${TOPDIR} LIBS=-L${TOPDIR}/lib/ CPPFLAGS=-I${TOPDIR}/include
make
make install
make distclean



