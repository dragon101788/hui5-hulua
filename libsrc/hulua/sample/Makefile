CC=$(HOME)/usr/arm/arm_linux_4.2/bin/arm-linux-gcc
PP=$(HOME)/usr/arm/arm_linux_4.2/bin/arm-linux-g++
all:
	$(PP) -I../include/  -L../lib/  sample1.cpp  -o sample1 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  sample2.cpp  -o sample2 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  sample3.cpp  -o sample3 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  sample4.cpp  -o sample4 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  sample5.cpp  -o sample5 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  sample6.cpp  -o sample6 -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  hu.cpp  -o hu -lhulua -lm -ldl -DLUA_32BITS
	$(PP) -I../include/  -L../lib/  cinvok.c  -o cinvok -lhulua  -DLUA_32BITS
