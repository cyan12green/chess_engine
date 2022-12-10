#include "board.h"

U64 Board::KnightAttacks(U64 knights) {
	U64 l1 = (knights >> 1) & U64(0x7f7f7f7f7f7f7f7f);
	U64 l2 = (knights >> 2) & U64(0x3f3f3f3f3f3f3f3f);
	U64 r1 = (knights << 1) & U64(0xfefefefefefefefe);
	U64 r2 = (knights << 2) & U64(0xfcfcfcfcfcfcfcfc);
	U64 h1 = l1 | r1;
	U64 h2 = l2 | r2;
	return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

void Board::initKnightAttacks() {
	Board::KnightAttacksLookup.clear();
	for (int i= 0; i<64; i++) {
		Board::KnightAttacksLookup.push_back( KnightAttacks(((U64)1 << i)) );
	}
}
