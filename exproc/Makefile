CC=arm-linux-g++
HUISRC=../
CFLAG=-I$(HUISRC)/ -I$(HUISRC)/include
OUTDIR=../../output/
all:
	$(CC) $(CFLAG) -fPIC test.cpp -shared -o $(OUTDIR)test.so
