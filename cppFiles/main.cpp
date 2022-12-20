#include "board.h"
#include "eval.h"
#include "engine.cpp"
#include <time.h>

#define DEPTH 6
// inits
vector<U64> Board::KnightAttacksLookup = {};
vector<U64> Board::KingAttacksLookup = {};
vector<vector<U64>> Board::RookBlockerTable(64, vector<U64>(4096, 0)) ;
vector<vector<U64>> Board::BishopBlockerTable(64, vector<U64>(4096,0));
vector<U64> Board::BishopMasks(64,0);
vector<U64> Board::RookMasks(64, 0);
vector<vector<int>> Eval::mg_table(12, vector<int>(64, 0));
vector<vector<int>> Eval::eg_table(12, vector<int>(64,0));

int main() {
	string fen = "rnbqkb1r/3pn2p/p2p2p1/1p3p2/1PP1p1P1/PQNP1PBP/3P4/R3K1NR w KQkq - 0 1";
	Board b;
	Board::init();
	Eval::initTables();
	b.FENtoBoard(fen);
	
	b.representBoard(WHITE);
	clock_t start, stop;

	vector<Board> engineMoves;
	
	start = clock();
	Engine::minimaxAB(b, DEPTH, true, WHITE, INT_MIN, INT_MAX, &engineMoves);
	stop = clock();
	
	cout << (float)(stop - start)/CLOCKS_PER_SEC << endl;
	

	reverse(engineMoves.begin(), engineMoves.end());
	Board from = b;
	for (Board c: engineMoves) {
		cout << Board::moveStr(from, c) << endl;
		from = c;
		c.representBoard(WHITE);
	}

	return 0;
}
