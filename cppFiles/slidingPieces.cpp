#include "board.h"


// https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html
void Board::initBishopMasks() {
	int sq;
	const U64 notEdge = ~(rank_i(8) | rank_i(1) | file_i(1) | file_i(8));
	for (int rank = 1; rank <= 8; rank++) {
		for (int file = 1; file <= 8; file++) {
			sq = (file-1 + 8*(rank-1));
			BishopMasks.push_back((_RAY.RAY[NORTHWEST][sq] | _RAY.RAY[SOUTHEAST][sq] | _RAY.RAY[NORTHEAST][sq] | _RAY.RAY[SOUTHWEST][sq]) 
					& notEdge);
		}
	}
}

void Board::initRookMasks() {
	const U64 notEdge = ~(rank_i(8) | rank_i(1) | file_i(1) | file_i(8));
	
	for (int rank =1; rank <=8; rank++) {
		for (int file = 1; file <= 8; file++) {
			RookMasks.push_back((file_i(file) | rank_i(rank)) & notEdge & ~square(file,rank));
		}
	}
}

U64 Board::generateAttackKeySliding(int piece, int sq, U64 blockers) {
	if (piece == BISHOP) {
		blockers &= BishopMasks[sq];
		return (blockers * BMagic[sq]) >> (64 - BishopBitShift[sq]);
	}
	//rook
	else {
		blockers &= RookMasks[sq];
		return (blockers * RMagic[sq]) >> (64 - RookBitShift[sq]);
	}
}
U64 Board::generateAttackSet(int piece, int sq, U64 blockers) {
	U64 attacks = 0ULL, temp;
	int blockerIndex;
	
	if (piece == BISHOP) {

		attacks |= _RAY.RAY[NORTHEAST][sq];
		if (_RAY.RAY[NORTHEAST][sq] & blockers) {
			temp= (_RAY.RAY[NORTHEAST][sq] & blockers);
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[NORTHEAST][blockerIndex];
		}
		
		attacks |= _RAY.RAY[NORTHWEST][sq];
		if (_RAY.RAY[NORTHWEST][sq] & blockers) {
			temp = _RAY.RAY[NORTHWEST][sq] & blockers;
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[NORTHWEST][blockerIndex];
		}

		attacks |= _RAY.RAY[SOUTHEAST][sq];
		if (_RAY.RAY[SOUTHEAST][sq] & blockers) {
			temp = (_RAY.RAY[SOUTHEAST][sq] & blockers);
			blockerIndex = __builtin_clzl(temp); 
			attacks &= ~_RAY.RAY[SOUTHEAST][63 - blockerIndex];
		}

		attacks |= _RAY.RAY[SOUTHWEST][sq];
		if (_RAY.RAY[SOUTHWEST][sq] & blockers) {
			temp = (_RAY.RAY[SOUTHWEST][sq] & blockers);
			blockerIndex = __builtin_clzl(temp);
			attacks &= ~_RAY.RAY[SOUTHWEST][63 - blockerIndex];
		} 


	}else if (piece == ROOK) {
		attacks |= _RAY.RAY[NORTH][sq];
		if (temp = _RAY.RAY[NORTH][sq] & blockers; temp) {
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[NORTH][blockerIndex];
		}
		attacks |= _RAY.RAY[EAST][sq];
		if (temp = _RAY.RAY[EAST][sq] & blockers; temp) {
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[EAST][blockerIndex];
		}
		attacks |= _RAY.RAY[SOUTH][sq];
		if (temp = _RAY.RAY[SOUTH][sq] & blockers; temp) {
			blockerIndex = __builtin_clzl(temp);
			attacks &= ~_RAY.RAY[SOUTH][63 - blockerIndex];
		}
		attacks |= _RAY.RAY[WEST][sq];
		if (temp = _RAY.RAY[WEST][sq] & blockers; temp) {
			blockerIndex = __builtin_clzl(temp);
			attacks &= ~_RAY.RAY[WEST][63 - blockerIndex];
		}

	}
	return attacks;
}
void Board::initBishopBlockerTable() {
	vector<U64> temp;
	int sq = 0;
	for (int rank = 1; rank <=8; rank++) {
		for (int file= 1; file <= 8; file++) {
			temp.clear();
			sq = (file-1) + 8*(rank-1);
			temp = permutBit(BishopMasks[sq]);
			for (U64 blockers: temp) {
				
				BishopBlockerTable[sq][Board::generateAttackKeySliding(BISHOP, sq, blockers)] = Board::generateAttackSet(BISHOP, sq, blockers); 
			}

		}
	}
}
void Board::initRookBlockerTable() {
	vector<U64> temp;
	int sq = 0;
	for (int rank = 1; rank <=8; rank++) {
		for (int file = 1; file <=8; file++) {
			temp.clear();
			sq = (file-1) + 8*(rank-1);
			temp = permutBit(RookMasks[sq]);
			for (U64 blockers: temp) {
				RookBlockerTable[sq][Board::generateAttackKeySliding(ROOK, sq, blockers)] = Board::generateAttackSet(ROOK, sq, blockers);
			}
		}
	}
}
