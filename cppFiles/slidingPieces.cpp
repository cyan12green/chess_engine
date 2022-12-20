#include "board.h"


// https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html
void Board::initRookMasks() {
	int rk, fl, f, r;
	U64 result;
	for (int sq = 0; sq < 64; sq++) {
		rk = sq/8;
		fl = sq % 8;
		result = 0ULL;
		for (r = rk+1; r <=6; r++) result |= (ONE << (fl + 8*r));
		for (r = rk-1; r >=1; r--) result |= (ONE << (fl + 8*r));
		for (f = fl+1; f <=6; f++) result |= (ONE << (f + 8*rk));
		for (f = fl-1; f >=1; f--) result |= (ONE << (f + 8*rk));
		RookMasks[sq] = result;
	} 
}

void Board::initBishopMasks() {
	int rk, fl, f, r;
	U64 result;
	for (int sq = 0; sq < 64; sq++) {
		rk = sq/8;
		fl = sq % 8;
		result = 0ULL;
		for (r = rk+1, f = fl+1; r <=6 && f <=6; r++, f++) result |= (ONE << (f + 8*r));
		for (r = rk-1, f = fl+1; r >=1 && f <=6; r--, f++) result |= (ONE << (f + 8*r));
		for (r = rk+1, f = fl-1; r <=6 && f >=1; r++, f--) result |= (ONE << (f + 8*r));
		for (r = rk-1, f = fl-1; r >=1 && f >=1; r--, f--) result |= (ONE << (f + 8*r));
		BishopMasks[sq] = result;
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
	U64 result = 0ULL;
	int rk = sq/8, fl = sq%8, r, f;
	if (piece == BISHOP) {
		for (r = rk+1, f = fl+1; r <=7 && f <=7; f++, r++) {
			result |= ONE << (f + 8*r);
			if (blockers & (ONE << (f + 8*r))) break;
		}
		for (r = rk+1, f = fl-1; r <=7 && f >=0; f--, r++) {
			result |= ONE << (f + 8*r);
			if (blockers & (ONE << (f + 8*r))) break;
		}
		for (r = rk-1, f = fl+1; f <=7 && r >=0; f++, r--) {
			result |= ONE << (f + 8*r);
			if (blockers & (ONE << (f + 8*r))) break;
		}
		for (r = rk-1, f = fl-1; f >=0 && r >=0; f--, r--) {
			result |= ONE << (f + 8*r);
			if (blockers & (ONE << (f + 8*r))) break;
		}
	}
	else {
		for (r= rk+1; r <= 7; r++) {
			result |= (ONE << (fl + 8*r));
			if (blockers & (ONE << (fl + 8*r))) break;
		}
		for (r= rk-1; r >= 0; r--) {
			result |= (ONE << (fl + 8*r));
			if (blockers & (ONE << (fl + 8*r))) break;
		}
		for (f = fl+1; f <= 7; f++) {
			result |= (ONE << (f + 8*rk));
			if (blockers & (ONE << (f + 8*rk))) break;
		}
		for (f = fl-1; f >= 0; f--) {
			result |= (ONE << (f + 8*rk));
			if (blockers & (ONE << (f + 8*rk))) break;
		}
	}
	return result;
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
