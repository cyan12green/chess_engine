#include "board.h"

//possible targets for single pawn push
U64 Board::PawnSinglePushTargets(U64 pawns, int color) {
	if (color == WHITE) {
		return moveOne(pawns, N) & empty();
	}
	else {
		return moveOne(pawns, S) & empty();
	}
}

//possible targets for double pawn push, not dealing with e.p yet
U64 Board::PawnDoublePushTargets(U64 pawns, int color) {
	U64 singlePushs = PawnSinglePushTargets(pawns, color);
	if (color == WHITE) {
		return moveOne(singlePushs, N) & empty() & rank_i(4); 
	}
	else
		return moveOne(singlePushs, S) & empty() & rank_i(5);
}

U64 Board::PawnsAbleToPush(U64 pawns, int color) {
	if (color == WHITE) {
		return moveOne(empty(), S) & pawns;
	}
	else {
		return moveOne(empty(), N) & pawns;
	}
}

U64 Board::PawnsAbleToDblPush(U64 pawns, int color) {
	if (color == WHITE) {
		U64 emptyRank3 = moveOne(empty() & rank_i(4), S) & empty();
		return moveOne(emptyRank3, S) & pawns;
	}
	else {
		U64 emptyRank6 = moveOne(empty() & rank_i(5), N) & empty();
		return moveOne(emptyRank6, N) & pawns;
	}
}


