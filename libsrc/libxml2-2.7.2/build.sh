#!/bin/sh

make clean
set -e
ARCH=arm 
CROSS_COMPILE=${HOME}/usr/arm/arm_linux_4.2/bin/arm-linux-
./configure --host=arm-linux --target=arm-linux CC=${CROSS_COMPILE}gcc AR=${CROSS_COMPILE}ar LD=${CROSS_COMPILE}ld ANLIB=${CROSS_COMPILE}ranlib --prefix=${PWD}/../

make install


