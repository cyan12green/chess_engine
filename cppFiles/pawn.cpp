#include "board.h"

//possible targets for single pawn push
U64 Board::PawnSinglePushTargets(U64 pawns, int color) {
	if (color == WHITE) {
		return moveOne(pawns, NORTH) & empty();
	}
	else {
		return moveOne(pawns, SOUTH) & empty();
	}
}

//possible targets for double pawn push, not dealing with e.p yet
U64 Board::PawnDoublePushTargets(U64 pawns, int color) {
	U64 singlePushs = PawnSinglePushTargets(pawns, color);
	if (color == WHITE) {
		return moveOne(singlePushs, NORTH) & empty() & rank_i(4); 
	}
	else
		return moveOne(singlePushs, SOUTH) & empty() & rank_i(5);
}

U64 Board::PawnsAbleToPush(U64 pawns, int color) {
	if (color == WHITE) {
		return moveOne(empty(), SOUTH) & pawns;
	}
	else {
		return moveOne(empty(), NORTH) & pawns;
	}
}

U64 Board::PawnsAbleToDblPush(U64 pawns, int color) {
	if (color == WHITE) {
		U64 emptyRank3 = moveOne(empty() & rank_i(4), SOUTH) & empty();
		return moveOne(emptyRank3, SOUTH) & pawns;
	}
	else {
		U64 emptyRank6 = moveOne(empty() & rank_i(5), NORTH) & empty();
		return moveOne(emptyRank6, NORTH) & pawns;
	}
}

U64 Board::PawnsEastAttacks(U64 pawns, int color) {
	if (color == WHITE) {
		return (pawns << 9) & ~(file_i(1));
	}
	else {
		return (pawns >> 7) & ~(file_i(1));
	}
}

U64 Board::PawnsWestAttacks(U64 pawns, int color) {
	if (color == WHITE) {
		return (pawns << 7) & ~(file_i(8));
	}
	else {
		return (pawns >> 9) & ~(file_i(8));
	}
}

U64 Board::PawnsAnyAttacks(U64 pawns, int color) {
	return PawnsEastAttacks(pawns, color) | PawnsWestAttacks(pawns, color);
}

U64 Board::PawnsDblAttacks(U64 pawns, int color) {
	return PawnsEastAttacks(pawns, color) & PawnsWestAttacks(pawns, color);
}

U64 Board::PawnsSingleAttacks(U64 pawns, int color) {
	return PawnsEastAttacks(pawns, color) ^ PawnsWestAttacks(pawns, color);
}

U64 Board::PawnsAbleToCaptureEast(U64 pawns, int color) {
	const U64 pieces = ((color == WHITE)? blackPieces() : whitePieces() );
	return pawns & PawnsWestAttacks(pieces, 1 - color);
}

U64 Board::PawnsAbleToCaptureWest(U64 pawns, int color) {
	const U64 pieces = ((color == WHITE)? blackPieces() : whitePieces() );
	return pawns & PawnsEastAttacks(pieces, 1 - color);
}

U64 Board::PawnsAbleToCaptureAny(U64 pawns, int color) {
	const U64 pieces = ((color == WHITE)? blackPieces() : whitePieces() );
	return pawns & PawnsAnyAttacks(pieces, 1 - color);
}




