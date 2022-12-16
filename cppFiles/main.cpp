#include "board.h"

// inits
vector<U64> Board::KnightAttacksLookup = {};
vector<U64> Board::KingAttacksLookup = {};
vector<vector<U64>> Board::RookBlockerTable(64, vector<U64>(4096, 0)) ;
vector<vector<U64>> Board::BishopBlockerTable(64, vector<U64>(4096,0));
vector<U64> Board::BishopMasks(64,0);
vector<U64> Board::RookMasks(64, 0);
Rays Board::_RAY = {};
int main() {
	string fen = "1r1qkb1r/pbpp1p1p/n4n2/1p2p1p1/P2P1P2/R1NBPN2/1PPB2PP/3QK2R w Kk e6 0 2";
	Board b;
	Board::init();
	
	b.FENtoBoard(fen);
	b.representBoard(WHITE);
	cout << b.enPassent << endl;
	cout << b.BoardtoFEN() << endl;
	for (Board c: Board::legalMoves(WHITE, b)) {
		cout << c.BoardtoFEN();
		cout << endl;
	}
	return 0;
}
