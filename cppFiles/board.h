#include <iostream>
#include <string>
#include "helperFile.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class Board {
	public:
		vector<Side> Players;
		bool WhiteToMove;
		int halfmove, fullmoveCounter;
		string enPassent;
		enum direction{N,E,S,W};

		//helpers
		Board();
		U64 square (int file, int rank);
		U64 empty();
		U64 whitePieces();
		U64 blackPieces();

		int whichOccupySide(int file, int rank);
		int whichOccupyPiece(int file, int rank, int color);
		U64 moveOne(U64 piece, int direction);
		void toBitboardString(int color, int piece);
		

		//Fen conversions
		void FENtoBoard(string fen);
		string BoardtoFEN();
		

		U64 PawnSinglePushTargets(U64 pawns, int color);
		U64 PawnDoublePushTargets(U64 pawns, int color);
		U64 PawnsAbleToPush(U64 pawns, int color);
		U64 PawnsAbleToDblPush(U64 pawns, int color);


};
