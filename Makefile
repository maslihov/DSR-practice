CC = gcc
CFLAG = -Wall -g 
LIBS = -lmenu -lpanel -lncurses
#LIBS = -lmenuw -lpanelw -lncursesw
SRC = ./src/
EXE = fm

.PHONY: assembly clean allclean nw

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

$(EXE):main.o fm.o panel.o dirc.o humanize.o
	$(CC) $(CFLAG) $(LIBS) -o $@ $^
	ls -lh $@

clean:  *.o
	rm -fv $^

allclean: *.o 
	rm -fv $(EXE) $^ 