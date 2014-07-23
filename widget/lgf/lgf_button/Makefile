CROSS_COMPILE:=arm-linux-
CC:=$(CROSS_COMPILE)g++
TOPDIR:=../../
CFLAG:=-I$(TOPDIR)/ -I$(TOPDIR)/include -DHUMODULE
OUTDIR:=./
TAG=$(OUTDIR)lgf_button.so
SRC=lgf_button.cpp

sinclude built-in.d
all:built-in.d built-in.o
	$(CC) $(CFLAG) -fPIC $< -shared -o $(TAG)

clean:
	-@rm *.d
	-@rm *.o
	-@rm *.so

built-in.d : $(SRC)
	@mkdir -p `dirname $@`
	@set -e; rm -f $@
	@$(CC) $(CFLAG) -MM $(CPPFLAGS) $< |sed 's/\\//g' > $@
	@echo $(patsubst %.d,%.o,$@) : `cut -f2 -d':' <$@` > $@
	@printf "\t$(CC) $(CFLAG) -c $(SRC) -o $(patsubst %.d,%.o,$@)\n" >>$@
