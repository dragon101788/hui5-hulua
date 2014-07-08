#.SUFFIXES : .x .o .c .s

#ROOT = ${HOME}/w55/arch/arm/include
#LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
#LIB1 =$(ROOT)/arm-linux/lib
#INCSYS:=$(ROOT)/arm-linux/sys-include
#INC :=$(ROOT)/arm-linux/include

#CC=/home/dragon/usr/arm/arm_linux_4.2/bin/arm-linux-gcc -O2 -I$(INC) -I$(INCSYS) 
PWD=./
export PATH=/usr/local/arm_linux_4.2/bin:$PATH
CC=arm-linux-gcc
AR=arm-linux-ar
#WEC_LDFLAGS=-L$(LIB) -L$(LIB1)
#STRIP=arm-linux-strip

TARGET = demo
SRCS := demo_main.c  TTF_func.c
#SRCS := hui.cpp codec.cpp blt.cpp hulib.cpp ParaseXML.cpp layer.cpp xmlproc.cpp
#SRCS += manager_touch.cpp manager_cs.cpp manager_timer.cpp

TTF_SRCS := ../trueType/*.c

OUTPUT =outdir

all: 

	$(CC) -c *.c
	$(AR) cr libruetype.a *.o
#	$(CC)  $(SRCS) $(TTF_SRCS) -o $(OUTPUT)/$(TARGET) 
# -lts -lpthread -lc -lgcc -ldl -lpng -rdynamic
	#$(CC)  nl.cpp -o $(OUTPUT)/nl
	#$(CC)  p.cpp -o $(OUTPUT)/p
	#$(CC)  getvar.cpp -o $(OUTPUT)/getvar
	#$(CC)  s.cpp -o $(OUTPUT)/s
	#$(CC)  line.cpp -o $(OUTPUT)/line

#	$(STRIP) $(TARGET) 
#	cp $(TARGET) ../../initramfs/usr/
#	@echo "Please also copy input patterns to storage"

clean:
	rm -f *.o 
	rm -f *.x 
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)	
	rm -f *.gdb
	rm -f *.bak
