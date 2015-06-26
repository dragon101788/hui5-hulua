
set -e
#CROSS_COMPILE=${HOME}/usr/arm/arm_linux_4.2/bin/arm-linux-

echo ${TOPDIR}
echo ${CROSS_COMPILE}


ROOTDIR=$(pwd)

cd ${ROOTDIR}/readline-6.2

./configure --prefix=${TOPDIR} CC=${CROSS_COMPILE}gcc --host=arm
make install

cd ${ROOTDIR}

make -f hulua.mk linux clean "CFLAGS=${CFLAGS} -I${TOPDIR}/include" LDFLAGS=-L${TOPDIR}/lib INSTALL_TOP=${TOPDIR} CROSS_COMPILE=${CROSS_COMPILE}
make -f hulua.mk linux install "CFLAGS=${CFLAGS} -I${TOPDIR}/include" LDFLAGS=-L${TOPDIR}/lib INSTALL_TOP=${TOPDIR} CROSS_COMPILE=${CROSS_COMPILE}
