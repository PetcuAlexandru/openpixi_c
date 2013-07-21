CC=gcc
CFLAGS=-g -Wall -lm
OBJ_REQ=Force.o Grid.o Simulation.o
TARGET=simulation

build:
	$(CC) Force.c Grid.c Simulation.c -o $(TARGET) -g -Wall -lm

Force.o: Force.c
	$(CC) $(CFLAGS) -c Force.c

Grid.o: Grid.c
	$(CC) $(CFLAGS) -c Grid.c

Simulation.o: Simulation.c
	$(CC) $(CFLAGS) -c Simulation.c

clean:
	rm -rf $(OBJ_REQ) $(TARGET) *~