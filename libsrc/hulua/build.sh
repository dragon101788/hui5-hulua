
#ROOTDIR=$(pwd)
#CROSS_COMPILE=${HOME}/usr/arm/arm_linux_4.2/bin/arm-linux-

echo ${TOPDIR}
echo ${CROSS_COMPILE}

ROOTDIR=${TOPDIR}/libsrc/hulua

cd ${ROOTDIR}/readline-6.2

./configure --prefix=${TOPDIR} CC=${CROSS_COMPILE}gcc --host=arm
make install

cd ${ROOTDIR}

make linux clean CFLAGS=-I${ROOTDIR}/include LDFLAGS=-L${ROOTDIR}/lib INSTALL_TOP=${TOPDIR} CROSS_COMPILE=${CROSS_COMPILE}
make linux install CFLAGS=-I${ROOTDIR}/include LDFLAGS=-L${ROOTDIR}/lib INSTALL_TOP=${TOPDIR} CROSS_COMPILE=${CROSS_COMPILE}
