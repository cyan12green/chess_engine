CC = g++
CFLAGS = -Wall -g

main: main.o board.o helperFile.o pawn.o
	$(CC) $(CFLAGS) -o main main.o board.o helperFile.o pawn.o

main.o: main.cpp board.h 
	$(CC) $(CFLAGS) -c main.cpp

board.o: board.h board.cpp 
	$(CC) $(CFLAGS) -c board.cpp

pawn.o: board.h pawn.cpp
	$(CC) $(CFLAGS) -c pawn.cpp

helperFile.o: helperFile.h helperFile.cpp 
	$(CC) $(CFLAGS) -c helperFile.cpp 

clean: 
	$(RM) *.o

