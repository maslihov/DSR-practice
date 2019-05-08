# Проект маленький поэтому пока так.
CC = gcc
CFLAG = -Wall -g
LIBS = -lmenu -lpanel -lncurses
LNW = -lmenuw -lpanelw -lncursesw
EXE = fm
SRCPATH = ./src/*.c

$(EXE): $(SRCPATH) ./src/*.h
	$(CC) $(CFLAG) $(LIBS) $(SRCPATH) -o $@
	ls -lh $@
nw: $(SRCPATH) ./src/*.h
	$(CC) $(CFLAG) $(LNW) $(SRCPATH) -o $(EXE)
	ls -lh $(EXE)
	
