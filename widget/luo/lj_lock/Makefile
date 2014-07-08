CROSS_COMPILE:=arm-linux-
CC:=$(CROSS_COMPILE)g++
TOPDIR:=../../src
CFLAG:=-I$(TOPDIR)/ -I$(TOPDIR)/include -DHUMODULE
OUTDIR:=./
TAG=$(OUTDIR)lj_lock.so
SRC=lj_lock.cpp

all:built-in.o
	$(CC) $(CFLAG) -fPIC $< -shared -o $(TAG)

clean:
	-@rm *.d
	-@rm *.o
	-@rm *.so

sinclude built-in.d
built-in.d : $(SRC)
	@mkdir -p `dirname $@`
	@set -e; rm -f $@
	@$(CC) $(CFLAG) -MM $(CPPFLAGS) $< |sed 's/\\//g' > $@
	@echo $(patsubst %.d,%.o,$@) : `cut -f2 -d':' <$@` > $@
	@printf "\t$(CC) $(CFLAG) -c $(SRC) -o $(patsubst %.d,%.o,$@)\n" >>$@
