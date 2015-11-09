CC=arm-linux-gnueabihf-gcc 
C_FLAGS=-std=c99 -Wall #-DDEBUG

SRC=parseGGA.c parseGSA.c gps.c
all:
	$(CC) $(C_FLAGS) $(SRC) -I. -o gps.o
clean:
	rm -f *.o
