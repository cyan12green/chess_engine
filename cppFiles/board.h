#define __BOARD__
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#ifndef __HELPERFILE__
#include "helperFile.h"
#endif

#ifndef __RAYS__
#include "rays.cpp"
#endif

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
class Board {
	public:
		vector<Side> Players;
		bool WhiteToMove;
		int halfmove, fullmoveCounter;
		string enPassent;
		
		static Rays _RAY;
		
		//inits
		Board();
		static void init();
		//helpers
		static U64 square (int file, int rank);
		U64 empty();
		U64 whitePieces();
		U64 blackPieces();

		int whichOccupySide(int file, int rank);
		int whichOccupyPiece(int file, int rank, int color);
		static U64 moveOne(U64 piece, int direction);
		void toBitboardString(int color, int piece);
		void toBitboardString(U64 piece);

		//Fen conversions
		void FENtoBoard(string fen);
		string BoardtoFEN();
		
		//Pawns
		U64 PawnSinglePushTargets(U64 pawns, int color);
		U64 PawnDoublePushTargets(U64 pawns, int color);
		U64 PawnsAbleToPush(U64 pawns, int color);
		U64 PawnsAbleToDblPush(U64 pawns, int color);
		U64 PawnsEastAttacks(U64 pawns, int color);
		U64 PawnsWestAttacks(U64 pawns, int color);
		U64 PawnsAnyAttacks(U64 pawns, int color);
		U64 PawnsDblAttacks(U64 pawns, int color);
		U64 PawnsSingleAttacks(U64 pawns, int color);
		U64 PawnsAbleToCaptureEast(U64 pawns, int color);
		U64 PawnsAbleToCaptureWest(U64 pawns, int color);
		U64 PawnsAbleToCaptureAny(U64 pawns, int color);
		
		
		//Knights
		static vector<U64> KnightAttacksLookup;
		static void initKnightAttacks();
		static U64 KnightAttacks(U64 knights);
		
		//King and castling
		static vector<U64> KingAttacksLookup;
		static U64 kingAttacks(U64 Kingset);
		static void initKingAttacks();
		bool canCastleKing(int color);
		bool canCastleQueen(int color);
		void castleKingSide(int color);
		void castleQueenSide(int color);
		
		//sliding pieces
		static vector<vector<U64>> RookBlockerTable; // RookBT[64][key]
		static vector<vector<U64>> BishopBlockerTable;// BishopBT[64][key]
		static vector<U64> BishopMasks; //Masks for diagnols 
		static vector<U64> RookMasks;
		static void initRookBlockerTable();
		static void initBishopBlockerTable();
		static void initBishopMasks();
		static void initRookMasks();
		static U64 generateAttackKeySliding(int piece, int sq, U64 blockers);
		static U64 generateAttackSet(int piece, int sq, U64 blockers);
};

