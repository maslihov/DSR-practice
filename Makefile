CC = gcc
CFLAG = -Wall -g 
LIBS =  -lform -lmenu -lpanel -lncurses
#LIBS = -lmenuw -lpanelw -lncursesw
SRC = ./src/
EXE = fm

.PHONY: assembly clean allclean

assembly: $(EXE)

main.o: $(SRC)main.c $(SRC)main.h
	$(CC) $(CFLAG) $(LIBS) -c -o $@ $<

fm.o: $(SRC)fm.c $(SRC)fm.h
	$(CC) $(CFLAG) $(LIBS) -c -o $@ $< 

panel.o: $(SRC)panel.c $(SRC)panel.h
	$(CC) $(CFLAG) $(LIBS) -c -o $@ $< 
	
dirc.o: $(SRC)dirc.c $(SRC)dirc.h
	$(CC) $(CFLAG) $(LIBS) -c -o $@ $< 
	
humanize.o: $(SRC)humanize.c $(SRC)humanize.h
	$(CC) $(CFLAG) $(LIBS) -c -o $@ $<

create.o: $(SRC)create.c $(SRC)create.h
	$(CC) $(CFLAG)  $(LIBS) -c -o $@ $<

delete.o:  $(SRC)delete.c  $(SRC)delete.h
	$(CC) $(CFLAG)  $(LIBS) -c -o $@ $<

rename.o:  $(SRC)rename.c  $(SRC)rename.h
	$(CC) $(CFLAG)  $(LIBS) -c -o $@ $<

copy.o:  $(SRC)copy.c  $(SRC)copy.h
	$(CC) $(CFLAG)  $(LIBS) -c -o $@ $<

cp.o: $(SRC)cp/cp.c $(SRC)cp/extern.h
	gcc -Wall -g -c -o $@ $<

utils.o: $(SRC)cp/utils.c $(SRC)cp/extern.h
	gcc -Wall -g -c -o $@ $<

viewer.o: $(SRC)viewer.c $(SRC)viewer.h
	gcc -Wall -g -c -o $@ $<

fm_err.o:  $(SRC)fm_err.c  $(SRC)fm_err.h
	$(CC) $(CFLAG)  $(LIBS) -c -o $@ $<

$(EXE):main.o fm.o panel.o dirc.o humanize.o create.o delete.o rename.o fm_err.o copy.o cp.o utils.o viewer.o
	$(CC) $(CFLAG) $(LIBS) -o $@ $^
	ls -lh $@

clean:  *.o
	rm -fv $^

allclean: *.o 
	rm -fv $(EXE) $^ 
