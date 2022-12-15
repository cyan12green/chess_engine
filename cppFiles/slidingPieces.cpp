#include "board.h"


// https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html
void Board::initBishopMasks() {
	int sq;
	const U64 notEdge = ~(rank_i(8) | rank_i(1) | file_i(1) | file_i(8));
	for (int rank = 1; rank <= 8; rank++) {
		for (int file = 1; file <= 8; file++) {
			sq = (file-1 + 8*(rank-1));
			BishopMasks[sq] = ((_RAY.RAY[NORTHWEST][sq] | _RAY.RAY[SOUTHEAST][sq] | _RAY.RAY[NORTHEAST][sq] | _RAY.RAY[SOUTHWEST][sq]) 
					& notEdge);
		}
	}
}

void Board::initRookMasks() {
	const U64 notEdge = ~(rank_i(8) | rank_i(1) | file_i(1) | file_i(8));
	const U64 notEdgeSq = ~(square(1,1) | square(8,1) | square(1,8) | square(8,8));
	int sq;
	for (int rank =1; rank <=8; rank++) {
		for (int file = 1; file <= 8; file++) {
			sq = file-1 + 8*(rank-1);
			RookMasks[sq] = ((file_i(file) | rank_i(rank)) & notEdge & ~square(file,rank));
			
			if (file == 1) {
				RookMasks[sq] |= (file_i(1) & ~square(file,rank));
			}
			if (file == 8) {
				RookMasks[sq] |= (file_i(8) & ~square(file,rank));
			}
			if (rank == 1) {
				RookMasks[sq] |= (rank_i(1) & ~square(file,rank));
			}
			if (rank == 8) {
				RookMasks[sq] |= (rank_i(8) & ~square(file,rank));
			}
			RookMasks[sq] &= notEdgeSq;
			 
		}
	}
	
}

int Board::generateAttackKeySliding(int piece, int sq, U64 blockers) {
	if (piece == BISHOP) {
		blockers &= BishopMasks[sq];
		return (int)((blockers * BMagic[sq]) >> (64 - BishopBitShift[sq]));
	}
	//rook
	else {
		blockers &= RookMasks[sq];
		return (int) ((blockers * RMagic[sq]) >> (64 - RookBitShift[sq]));
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
		temp = (_RAY.RAY[NORTH][sq] & blockers);
		if (temp) {
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[NORTH][blockerIndex];
		}
		attacks |= _RAY.RAY[EAST][sq];
		temp = _RAY.RAY[EAST][sq] & blockers; 
		if (temp) {
			blockerIndex = pop_1st_bit(&temp);
			attacks &= ~_RAY.RAY[EAST][blockerIndex];
		}
		attacks |= _RAY.RAY[SOUTH][sq];
		temp = _RAY.RAY[SOUTH][sq] & blockers;
		if (temp) {
			blockerIndex = __builtin_clzl(temp);
			attacks &= ~_RAY.RAY[SOUTH][63 - blockerIndex];
		}
		attacks |= _RAY.RAY[WEST][sq];
		temp = _RAY.RAY[WEST][sq] & blockers;
		if (temp) {
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

U64 Board::getMovesSlidingPiece(int piece, int sq, U64 blockers) {
	if (piece == BISHOP) {
		return BishopBlockerTable[sq][generateAttackKeySliding(BISHOP, sq, blockers)];
	}
	else if (piece == ROOK) {
		return RookBlockerTable[sq][generateAttackKeySliding(ROOK, sq, blockers)];
	}
	else {
		return BishopBlockerTable[sq][generateAttackKeySliding(BISHOP, sq, blockers)] | RookBlockerTable[sq][generateAttackKeySliding(ROOK,sq,blockers)]; 
	}
}
