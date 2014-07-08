
LXDIALOG += lxdialog/textbox.c
LXDIALOG += lxdialog/yesno.c
LXDIALOG += lxdialog/menubox.c
LXDIALOG += lxdialog/inputbox.c
LXDIALOG += lxdialog/util.c
LXDIALOG += lxdialog/checklist.c

OUTDIR:=./



all: 
	gcc mconf.c zconf.tab.c $(LXDIALOG) -o $(OUTDIR)mconf -DCURSES_LOC="<ncurses.h>" -lncurses
	gcc conf.c zconf.tab.c $(LXDIALOG) -o $(OUTDIR)conf -DCURSES_LOC="<ncurses.h>" -lncurses
clean:
	
	
