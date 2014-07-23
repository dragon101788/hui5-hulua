CC:=arm-linux-g++
STRIP:=arm-linux-strip
OUTPUT:=/mnt/hgfs/giant_share/joyoung/output/


all:
	$(CC)  $(CFLAG) updatetime.cpp -o $(OUTPUT)/updatetime
	$(CC)  $(CFLAG) nl.cpp -o $(OUTPUT)/nl
	$(CC)  $(CFLAG) p.cpp ../hulib.cpp -o $(OUTPUT)/p -ldl
	$(CC)  $(CFLAG) g.cpp ../hulib.cpp -o $(OUTPUT)/g -ldl
	$(CC)  $(CFLAG) getvar.cpp -o $(OUTPUT)/getvar
	$(CC)  $(CFLAG) line.cpp -o $(OUTPUT)/line



