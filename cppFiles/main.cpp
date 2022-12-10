#include "board.h"

// inits
vector<U64> Board::KnightAttacksLookup = {};
vector<U64> Board::KingAttacksLookup = {};
vector<vector<U64>> Board::RookBlockerTable(64, vector<U64>(4096)) ;
vector<vector<U64>> Board::BishopBlockerTable(64, vector<U64>(4096));
vector<U64> Board::BishopMasks = {};
vector<U64> Board::RookMasks = {};
Rays Board::_RAY = {};
int main() {
	Board b;
	Board::init();
	b.FENtoBoard(START_FEN);
	int ind= 0;
	int sq = 22;
	U64 blockers = 0xFFED345AB1;
	cout << "blockers \n";
	b.toBitboardString(blockers);
	
	cout << "rook mask \n";
	b.toBitboardString(b.RookMasks[sq]);
	
	cout << "masked blocker \n";
	b.toBitboardString(blockers & Board::RookMasks[sq]);
	//b.toBitboardString((blockers & Board::BishopMasks[sq]) * BMagic[sq]);
	
	//b.toBitboardString(Board::generateAttackSet(BISHOP, sq, blockers));
	cout << "attack set \n";
	b.toBitboardString(Board::generateAttackSet(ROOK,sq, blockers));//Board::RookBlockerTable[sq][Board::generateAttackKeySliding(ROOK, sq, blockers)]);
	
	cout << "done";

	return 0;
}
