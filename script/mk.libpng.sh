#!/bin/sh


cd ${LIBSRCDIR}/libpng-1.6.12
echo !!!!!!!!!${TOPDIR}!!!!!!!!!1
make distclean
./configure --host=${HOST} CC=${CROSS_COMPILE}gcc --prefix=${TOPDIR} LIBS=-L${TOPDIR}/lib/ CPPFLAGS=-I${TOPDIR}/include
make
make install
make distclean



