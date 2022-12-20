#include "board.h"

U64 Board::kingAttacks(U64 kingSet) {
	U64 attacks = Board::moveOne(kingSet, EAST) | Board::moveOne(kingSet, WEST); //East and West
	kingSet |= attacks;
	attacks |= Board::moveOne(kingSet, NORTH) | Board::moveOne(kingSet, SOUTH); //North and South
	return attacks;
}
void Board::initKingAttacks() {
	U64 sqBB = 1;
	Board::KingAttacksLookup.clear();
	for (int sq =0; sq <64; sq++, sqBB<<=1) {
		Board::KingAttacksLookup.push_back(kingAttacks(sqBB));
	}
}


bool Board::canCastleKing(int color) {
	if (color == WHITE) { 
		return Players[WHITE].CastleKing && (whichOccupySide(6, 1) == -1) && (whichOccupySide(7,1) == -1) 
			&& !isSqAttackedBy(square(6,1), BLACK, *this) && !isSqAttackedBy(square(7,1), BLACK, *this);
	}
	else {
		return Players[BLACK].CastleKing && (whichOccupySide(6,8) == -1) && (whichOccupySide(7,8) == -1) 
			&& !isSqAttackedBy(square(6,8),WHITE, *this) && !isSqAttackedBy(square(7,8), WHITE, *this);
	}
}

bool Board::canCastleQueen(int color) {
	if (color == WHITE) {
		return Players[WHITE].CastleQueen && (whichOccupySide(2, 1) == -1) && (whichOccupySide(3,1) == -1) && (whichOccupySide(4,1) == -1)			&& !isSqAttackedBy(square(4,1), BLACK, *this) && !isSqAttackedBy(square(3,1), BLACK, *this);
	}
	else {
		return Players[BLACK].CastleKing && (whichOccupySide(2,8) == -1) && (whichOccupySide(3,8) == -1) && (whichOccupySide(4,8) == -1)&& !isSqAttackedBy(square(4,8), WHITE, *this) && !isSqAttackedBy(square(3,8), WHITE, *this);
	}
}

void Board::castleKingSide(int color) {
	Players[color].Pieces[KING] <<=2;
	if (color == WHITE) {
		Players[color].Pieces[ROOK] &= ~((U64) 1 << 7 );
		Players[color].Pieces[ROOK] |= ((U64)1 << 5);
	}
	else {
		Players[color].Pieces[ROOK] &= ~((U64) 1 << 63 );
		Players[color].Pieces[ROOK] |= ((U64)1 << 61);
	} 
	Players[color].CastleKing = false;
}

void Board::castleQueenSide(int color) {
	Players[color].Pieces[KING] >>=2;
	if (color == WHITE) {
		Players[color].Pieces[ROOK] &= ~((U64) 1 << 0 );
		Players[color].Pieces[ROOK] |= ((U64)1 << 3);
	}
	else {
		Players[color].Pieces[ROOK] &= ~((U64) 1 << 56 );
		Players[color].Pieces[ROOK] |= ((U64)1 << 59);
	}
	Players[color].CastleQueen = false;
}

bool Board::isInCheck(int color) {
	U64 temp = Players[color].Pieces[KING];
	return isSqAttackedBy(pop_1st_bit(&temp), 1-color, *this);
}

bool Board::isCheckMate() {
	if (WhiteToMove) {
		return isInCheck(WHITE) && (legalMoves(WHITE, *this).empty());
	}
	else {
		return isInCheck(BLACK) && (legalMoves(BLACK, *this).empty());
	}
}
