CC:=g++
TOPDIR:=../../
CFLAG:=-I$(TOPDIR)/ -I$(TOPDIR)/include -I$(TOPDIR)/mres
OUTPUT:=$(TOPDIR)/../output
SRC+=packres.cpp 
SRC+=$(TOPDIR)/codec.cpp 
SRC+=$(TOPDIR)/platfrom/alpha_soft.cpp 
SRC+=$(TOPDIR)/HumapXML.cpp 
SRC+=lib/libpng.a 
SRC+=lib/libxml2.a 
LDFLAG:=-lpthread -lrt -lz

all: lib/libpng.a lib/libxml2.a
	$(CC)  $(CFLAG) $(SRC) $(LDFLAG) -o $(OUTPUT)/packres

lib/libpng.a:
	LIBSRCDIR=$(TOPDIR)/libsrc TOPDIR=$(PWD)  $(TOPDIR)/script/mk.libpng.sh 

lib/libxml2.a:
	LIBSRCDIR=$(TOPDIR)/libsrc TOPDIR=$(PWD)  $(TOPDIR)/script/mk.xml2.sh 
