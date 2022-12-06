#include "board.h"

int main() {
	Board b;
	b.FENtoBoard(START_FEN);
	b.toBitboardString(WHITE, PAWN);
	return 0;
}
