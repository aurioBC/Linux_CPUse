# compilation variables
CC = gcc
CFLAGS = -W

FILES = src/main.c src/fileio.c src/cpu.c src/ui.c
EXE = bin/cpuse

proxy: $(FILES)
	$(CC) $(CFLAGS) -o $(EXE) $(FILES) -lncurses

clean:
	rm -f $(EXE)
