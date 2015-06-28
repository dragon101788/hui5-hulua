#CROSS_COMPILE=$(HOME)/usr/arm/arm_linux_4.2/bin/arm-linux-
CC=$(CROSS_COMPILE)g++
CFLAG=-I$(HOME)/hui/src/include/
all:
	#make -C ../ TOPDIR=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE)
	$(CC) $(CFLAG) -L./lib   lua.c -lhulua -lm -ldl -lreadline -ltermcap -o bin/lua
	$(CC) $(CFLAG) -L./lib   sample1.cpp   -o sample1 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample2.cpp   -o sample2 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample3.cpp   -o sample3 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample4.cpp   -o sample4 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample5.cpp   -o sample5 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample6.cpp   -o sample6 -lhulua -lm -ldl -DLUA_C89_NUMBERS
	$(CC) $(CFLAG) -L./lib   sample7.cpp   -o sample7 -lhulua -lm -ldl -DLUA_C89_NUMBERS
