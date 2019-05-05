# Проект маленький поэтому пока так.
CC = gcc
CFLAG = -Wall -g
LIBS = -lmenu -lpanel -lncurses
EXE = fm
SRCPATH = ./src/*.c

$(EXE): $(SRCPATH) ./src/*.h
	$(CC) $(CFLAG) $(LIBS) $(SRCPATH) -o $@
	ls -lh $@
	
