#CROSS_COMPILE=$(HOME)/usr/arm/arm_linux_4.2/bin/arm-linux-
CC=$(CROSS_COMPILE)g++
all:
	make -C ../ TOPDIR=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE)
	$(CC) -I./include/ -L./lib   sample1.cpp   -o sample1 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample2.cpp   -o sample2 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample3.cpp   -o sample3 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample4.cpp   -o sample4 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample5.cpp   -o sample5 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample6.cpp   -o sample6 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) -I./include/ -L./lib   sample7.cpp   -o sample7 -lhulua -lm -ldl -DLUA_C89_NUMBERS
