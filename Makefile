sinclude .config
CROSS_COMPILE?=$(HOME)/usr/arm/arm_linux_4.2/bin/arm-linux-
HOST?=arm-linux

#CROSS_COMPILE:=
#HOST:=

CC=$(CROSS_COMPILE)g++
STRIP=$(CROSS_COMPILE)strip
TOPDIR=$(PWD)/
FREETYPE=$(TOPDIR)/include/freetype2
CFLAG+= -DLUA_C89_NUMBERS
CFLAG+=-I$(TOPDIR)
CFLAG+=-I$(TOPDIR)include
CFLAG+=-I$(FREETYPE)
LDFLAG += -L$(TOPDIR)/lib/
LDFLAG += -lpthread -lc -lgcc -ldl -rdynamic -lrt -lhulua -lreadline -ltermcap
OUTPUT =../output/
TARGET = $(OUTPUT)hui
OBJS_DIR = $(TOPDIR)/objs/
DRAGON_AUTO = $(TOPDIR)/script/dragon_auto.sh
LIBSRCDIR := $(PWD)/libsrc
MCONF = $(TOPDIR)/script/mconf
CONF = $(TOPDIR)/script/conf
MKZLIB = $(TOPDIR)/script/mk.zlib.sh
MKPNGLIB = $(TOPDIR)/script/mk.libpng.sh
MKICONVLIB = $(TOPDIR)/script/mk.iconv.sh
MKXML2LIB = $(TOPDIR)/script/mk.xml2.sh
MKFREETYPELIB = $(TOPDIR)/script/mk.freetype.sh
HULUA = $(TOPDIR)/libsrc/hulua/
MKAUTO=Makefile.auto
BUILTIN_LIB=lib/libpng.a lib/libz.a lib/libfreetype.a lib/libxml2.a lib/libhulua.a
MAKE=make CROSS_COMPILE=$(CROSS_COMPILE) CC=$(CC) CFLAG="$(CFLAG)" TOPDIR=$(TOPDIR)

obj-y += hui_hulua.o
obj-y += XMLInstal.o
obj-y += loaderDL.o
obj-$(CONFIG_ALPHA_BLT) += platfrom/alpha_w55.o
obj-$(CONFIG_ALPHA_SOFT) += platfrom/alpha_soft.o
obj-y += thread_timer.o
obj-y += thread_touch.o
obj-y += manager_timer.o
obj-y += manager_touch.o
obj-y += schedule.o
obj-y += xmlproc.o
obj-y += HumapXML.o
obj-y += hulib.o
obj-y += codec.o
obj-y += Framebuffer.o
obj-y += lua_command.o
obj-y += hui.o 
obj-$(CONFIG_TOUCH_EKTF2K) += platfrom/touch_ektf2k.o
obj-$(CONFIG_TOUCH_NONE) += platfrom/touch_none.o

ifeq ($(CONFIG_USING_FONT),y) 

	obj-$(CONFIG_USING_FONT) += ttf_font.o
	obj-$(CONFIG_USING_FONT) += transcoding.o
else
endif 



sinclude  $(MKAUTO)


OBJS := $(patsubst %.o,$(OBJS_DIR)%.o,$(obj-y))
DEPS := $(patsubst %.o,$(OBJS_DIR)%.d,$(obj-y))
dir-objs=$(patsubst %,%/built-in.o,$(dir-y))

OBJS_MK=$(CC) $(CFLAG) -c $< -o $@
%.o : %.c
	proc gcc
	@mkdir -p `dirname $@`
	$(CC) $(CFLAG) -c $< -o $@

%.o : %.cpp
	proc gcc
	@mkdir -p `dirname $@`
	$(CC) $(CFLAG) -c $< -o $@

.PHONY: all
all: .config $(BUILTIN_LIB) dragon_auto $(DEPS) $(OBJS) dirobjs
	@echo built-in module: $(patsubst $(TOPDIR)%,%,$(dir-y))
	@$(CC)  $(OBJS) $(SRCS) $(BUILTIN_LIB) $(dir-objs) -o $(TARGET) $(LDFLAG)
	@#rm $(dir-objs)
	@#$(STRIP) $(TARGET) 
	@echo build done 	
	@rm Makefile.auto
	@rm Kconfig.auto

.PHONY: dragon_auto
dragon_auto:
	@TOPDIR=$(TOPDIR) $(DRAGON_AUTO)

lib/libhulua.a:
	echo build hulib
	make -C $(HULUA) TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE)
	rm $(TOPDIR)/bin -rf
	rm $(TOPDIR)/share -rf
	rm $(TOPDIR)/man/ -rf

lib/libz.a:
	@mkdir -p `dirname $@`
	LIBSRCDIR=$(LIBSRCDIR) CROSS_COMPILE=$(CROSS_COMPILE) $(MKZLIB)
	rm $(TOPDIR)/bin -rf
	rm $(TOPDIR)/share -rf
lib/libpng.a: lib/libz.a
	@mkdir -p `dirname $@`
	LIBSRCDIR=$(LIBSRCDIR) TOPDIR=$(TOPDIR) HOST=$(HOST) CROSS_COMPILE=$(CROSS_COMPILE) $(MKPNGLIB)
	rm $(TOPDIR)/bin -rf
	rm $(TOPDIR)/share -rf


lib/libxml2.a:
	LIBSRCDIR=$(LIBSRCDIR) TOPDIR=$(TOPDIR) HOST=$(HOST) CROSS_COMPILE=$(CROSS_COMPILE) $(MKXML2LIB)

lib/libfreetype.a:
	LIBSRCDIR=$(LIBSRCDIR) TOPDIR=$(TOPDIR) HOST=$(HOST) CC=$(CROSS_COMPILE)gcc $(MKFREETYPELIB)
	rm $(TOPDIR)/bin -rf
	rm $(TOPDIR)/share -rf
	
.PHONY: tools
tools:
	$(MAKE) -C tools/

$(dir-objs):
	@mkdir -p `dirname $@`
	@$(MAKE) -C $(patsubst %built-in.o,%,$@) built-in.o

.PHONY:
dirobjs:
	@for dir in $(dir-y);do echo build $$dir ;$(MAKE) -s -C $$dir built-in.o;done

$(MKAUTO):
	@TOPDIR=$(TOPDIR) $(DRAGON_AUTO)

$(MCONF) : 
	make OUTDIR=$(dir $@) -C script/kconfig

$(CONF) : 
	make OUTDIR=$(dir $@) -C script/kconfig

#.config: $(MCONF) $(CONF)
#	echo $(WIDGETDIR)
#	$(MCONF) Kconfig
#	$(CONF) -s Kconfig

.PHONY: menuconfig
menuconfig: $(MCONF) $(CONF) dragon_auto
	echo $(WIDGETDIR)
	$(MCONF) Kconfig
	$(CONF) -s Kconfig
	@rm Makefile.auto
	@rm Kconfig.auto

.PHONY: clean
clean:
	-rm -rf $(OBJS_DIR)
	-rm -f $(TARGET)	
	-@for dir in $(dir-y);do $(MAKE) -s -C $$dir clean;done

.PHONY: distclean
distclean: clean
	-rm $(MCONF)
	-rm $(CONF)
	-rm -rf lib

sinclude $(DEPS)

$(OBJS_DIR)%.d : %.cpp
	@mkdir -p `dirname $@`
	@set -e; rm -f $@
	@$(CC) $(CFLAG) -MM $(CPPFLAGS) $< |sed 's/\\//g' > $@
	@echo $(patsubst %.d,%.o,$@) : `cut -f2 -d':' <$@` > $@
	@printf "\t$(CC) $(CFLAG) -c $(patsubst %.d,%.cpp,$<) -o $(patsubst %.d,%.o,$@)\n" >>$@

$(OBJS_DIR)%.d : %.c
	@mkdir -p `dirname $@`
	@set -e; rm -f $@
	@$(CC) $(CFLAG) -MM $(CPPFLAGS) $< |sed 's/\\//g' > $@
	@echo $(patsubst %.d,%.o,$@) : `cut -f2 -d':' <$@` > $@
	@printf "\t$(CC) $(CFLAG) -c $(patsubst %.d,%.c,$<) -o $(patsubst %.d,%.o,$@)\n" >>$@
	
