CC = g++
CFLAGS = -Wall -g -O3

main: main.o board.o helperFile.o pawn.o knight.o king.o slidingPieces.o move.o eval.o engine.o
	$(CC) $(CFLAGS) -o main main.o board.o helperFile.o pawn.o knight.o king.o slidingPieces.o move.o eval.o engine.o

main.o: main.cpp board.h 
	$(CC) $(CFLAGS) -c main.cpp

board.o: board.h board.cpp  
	$(CC) $(CFLAGS) -c board.cpp

pawn.o: board.h pawn.cpp
	$(CC) $(CFLAGS) -c pawn.cpp

knight.o: board.h knight.cpp
	$(CC) $(CFLAGS) -c knight.cpp

king.o: board.h king.cpp
	$(CC) $(CFLAGS) -c king.cpp

helperFile.o: helperFile.h helperFile.cpp 
	$(CC) $(CFLAGS) -c helperFile.cpp 

slidingPieces.o: slidingPieces.cpp board.h
	$(CC) $(CFLAGS) -c slidingPieces.cpp

move.o: move.cpp board.h
	$(CC) $(CFLAGS) -c move.cpp

eval.o: eval.cpp eval.h 
	$(CC) $(CFLAGS) -c eval.cpp

engine.o: engine.cpp eval.h board.h
	$(CC) $(CFLAGS) -c engine.cpp
clean: 
	$(RM) *.o main; clear;
