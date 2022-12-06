#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <ctype.h>
#include <cstdint>

typedef uint64_t U64;

using namespace std;
#define PAWN 0
#define BISHOP 1
#define KNIGHT 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#define WHITE 0
#define BLACK 1

#define MAXPIECES 6 

const U64 file_mask = 0x8080808080808080ull;

struct Side {
	vector<U64> Pieces;
	bool CastleKing, CastleQueen;
};

vector<string> splitString(string str, string delim);


U64 rank_i(int i);

U64 file_i(int i);
