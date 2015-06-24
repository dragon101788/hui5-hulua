#!/bin/sh


cd libsrc/libiconv-1.14
echo !!!!!!!!!${TOPDIR}!!!!!!!!!1
make distclean
./configure --host=${HOST} CC=${CROSS_COMPILE}gcc --prefix=${TOPDIR} LIBS=-L${TOPDIR}/lib/ CPPFLAGS=-I${TOPDIR}/include --enable-static
make
make install
make distclean



