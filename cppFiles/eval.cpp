#include "eval.h"
void Eval::initTables() {
	int pc, p, sq;
	for (p = PAWN, pc = WHITE_PAWN; p <= KING; pc += 2, p++) {
		for (sq = 0; sq < 64; sq++) {
			mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][sq];
			eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][sq];
			mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
			eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
		}
	}
}
int Eval::evaluate(Board b) {
	/*if (b.isCheckMate()) {
		return INT_MIN;
	}*/
	int mg[2];
	int eg[2];
	int gamePhase = 0;
	int sq = -1;
	U64 piece;
	mg[WHITE] = 0;
	mg[BLACK] = 0;
	eg[WHITE] = 0;
	eg[BLACK] = 0;
	
	int side2move = WHITE;//(b.WhiteToMove)?WHITE:BLACK;

	for (int pc = 0; pc < MAXPIECES; pc++) {
		for (int col = 0; col <2; col++) {
			piece = b.Players[col].Pieces[pc];
			while(piece) {
				sq = pop_1st_bit(&piece);
				mg[col] += mg_table[2*pc + col][sq];
				eg[col] += eg_table[2*pc + col][sq];
				gamePhase += gamephaseInc[2*pc + col];
			}
		}
	}

	int mgScore = mg[side2move] - mg[OTHER(side2move)];
	int egScore = eg[side2move] - eg[OTHER(side2move)];
	int mgPhase = gamePhase;
	if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
	int egPhase = 24 - mgPhase;
	return (mgScore * mgPhase + egScore * egPhase) / 24;
}
