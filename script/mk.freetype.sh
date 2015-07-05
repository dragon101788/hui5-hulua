#!/bin/sh

cd ${LIBSRCDIR}/freetype/
echo !!!!!!!!!${TOPDIR}!!!!!!!!!
echo !!!!!!!!!${MC_HOST}!!!!!!!!!
echo !!!!!!!!!${LIBS_CC}!!!!!!!!!

make distclean

#export CC=${LIBS_CC}

./configure --host=${HOST}  --prefix=${TOPDIR}  --without-png --without-zlib


make
make install
make distclean




