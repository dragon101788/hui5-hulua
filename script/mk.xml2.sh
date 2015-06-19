#!/bin/sh

cd libsrc/libxml2-2.7.2/
make distclean
set -e
./configure \
	--with-minimum \
	--with-sax1 \
	--without-zlib --host=arm-linux --target=arm-linux \
	CC=${CROSS_COMPILE}gcc \
	AR=${CROSS_COMPILE}ar \
	LD=${CROSS_COMPILE}ld \
	ANLIB=${CROSS_COMPILE}ranlib \
	--prefix=${TOPDIR}/objs/ \



make install
cp -rf ${TOPDIR}/objs/include/libxml2/libxml ${TOPDIR}/include/ 
cp -rf ${TOPDIR}/objs/lib/* ${TOPDIR}/lib/ 
rm ${TOPDIR}/objs/lib/ -rf
rm ${TOPDIR}/objs/include/ -rf
make distclean

