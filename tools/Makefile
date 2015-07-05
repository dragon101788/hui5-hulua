CC:=g++
CFLAG:=-I../ -I../include
OUTPUT:=../../output
SRC:=packres.cpp ../codec.cpp ../hulib.cpp ../platfrom/alpha_soft.cpp ../HumapXML.cpp lib/libpng.a lib/libxml2.a -lpthread -lrt -lz

all: lib/libpng.a lib/libxml2.a
	$(CC)  $(CFLAG) $(SRC) -o $(OUTPUT)/packres

lib/libpng.a:
	LIBSRCDIR=$(PWD)/../libsrc TOPDIR=$(PWD)  ../script/mk.libpng.sh 

lib/libxml2.a:
	LIBSRCDIR=$(PWD)/../libsrc TOPDIR=$(PWD)  ../script/mk.xml2.sh 
