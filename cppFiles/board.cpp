#include "board.h"
using namespace std;

//constructor
Board::Board() {
	for (int i = 0; i < 2; i++) {
		Players.push_back(Side());
		for (int j = 0; j < MAXPIECES; j++) {
			Players[i].Pieces.push_back(0);
		}
		Players[i].CastleKing = Players[i].CastleQueen = true;
	}
	halfmove = fullmoveCounter = 0;
	WhiteToMove = true;
	enPassent = '-';
}

Board::Board(const Board &b) {
	for (int i=0; i < 2; i++) Players.push_back(Side());
	
	Players[WHITE].Pieces.insert(Players[WHITE].Pieces.begin(), b.Players[WHITE].Pieces.begin(), b.Players[WHITE].Pieces.end());
	Players[BLACK].Pieces.insert(Players[BLACK].Pieces.begin(), b.Players[BLACK].Pieces.begin(), b.Players[BLACK].Pieces.end());
	
	halfmove = b.halfmove;
	fullmoveCounter = b.fullmoveCounter;
	WhiteToMove = b.WhiteToMove;
	enPassent = b.enPassent;
}

void Board::init() {
	Board::initKnightAttacks();
	Board::initKingAttacks();
	Board::initBishopMasks();
	Board::initRookMasks();
	Board::initRookBlockerTable();
	Board::initBishopBlockerTable();
}

U64 Board::square (int file, int rank) {
	return ((U64)1) << ( (8 * rank) + file - 9) ;
}

// convert the fen to the bitboard
void Board::FENtoBoard (string fen) {
	vector<string> whole = splitString(fen, " ");
	vector<string> piecesInFEN = splitString(whole[0], "/"); // just the pieces in fen

	WhiteToMove = (whole[1] == "w");
	if (whole[3] == "-") {
		enPassent = 0;
	}
	else {
		enPassent = square(whole[3][0] - 'a' + 1, whole[3][1] - '1' + 1);
	}

	halfmove = stoi(whole[4]);
	fullmoveCounter = stoi(whole[5]);

	// first part of FEN
	int rank = 8;
	int file;
	for (auto con: piecesInFEN) {
		file = 1;
		for (auto s: con) {
			if (isdigit(s)) {
				file += (s - '0');
			}
			else {
				int who = (isupper(s))?WHITE:BLACK;

				s = toupper(s);
				int foundPiece = -1;
				switch(s) {
					case 'P':
						foundPiece = PAWN;
						break;
					case 'B':
						foundPiece = BISHOP;
						break;
					case 'N':
						foundPiece = KNIGHT;
						break;
					case 'R':
						foundPiece = ROOK;
						break;
					case 'Q':
						foundPiece = QUEEN;
						break;
					case 'K':
						foundPiece = KING;
						break;
				}
				Players[who].Pieces[foundPiece] += square(file, rank);

				file++;
			}
		}

		rank--;
	}

	//castled part
	Players[WHITE].CastleKing = (whole[2].find("K") != string::npos);
	Players[WHITE].CastleQueen = (whole[2].find("Q") != string::npos);
	Players[BLACK].CastleKing = (whole[2].find("k") != string::npos);
	Players[BLACK].CastleQueen = (whole[2].find("q") != string::npos);
}

// convert bitboards to FEN
string Board::BoardtoFEN() {
	string resultString = "";
	int counter, sideOccupying, pieceOccupying;
	char ch = '-';
	for (int rank = 8; rank > 0; rank--) {
		counter = 0;
		for (int file = 1; file <= 8; file++) {
			sideOccupying = whichOccupySide(file, rank);
			if (sideOccupying == -1) {
				counter++;
			}
			else {
				if (counter != 0) {
					resultString.push_back(counter + '0');
					counter = 0;
				}
				pieceOccupying = whichOccupyPiece(file, rank, sideOccupying);
				switch(pieceOccupying) {
					case PAWN:
						ch = 'p';
						break;
					case BISHOP:
						ch = 'b';
						break;
					case KNIGHT:
						ch = 'n';
						break;
					case ROOK:
						ch = 'r';
						break;
					case QUEEN:
						ch = 'q';
						break;
					case KING:
						ch = 'k';
						break;
				}
				ch = ((sideOccupying == WHITE)? toupper(ch):tolower(ch));
				resultString.push_back(ch);
			}
		}
		if (counter != 0) {
			resultString.push_back(counter + '0');
		}
		resultString.push_back('/');
	}
	resultString.pop_back();

	resultString.push_back(' ');
	ch = (WhiteToMove)?'w':'b';
	resultString.push_back(ch);
	resultString.push_back(' ');


	if (Players[WHITE].CastleKing) 
		resultString.push_back('K');
	if (Players[WHITE].CastleQueen) 
		resultString.push_back('Q');
	if (Players[BLACK].CastleKing) 
		resultString.push_back('k');
	if (Players[BLACK].CastleQueen) 
		resultString.push_back('q');

	resultString.push_back(' ');
	U64 enPT = enPassent;
	int temp = pop_1st_bit(&enPT);
	if (enPassent != 0)
		resultString.append((char)('a' + (temp % 8)) + to_string(temp/8 + 1));
	else
		resultString.append("-");

	resultString.push_back(' ');
	resultString.append(to_string(halfmove) + " " + to_string(fullmoveCounter));
	return resultString;
}

// returns which side is occupied in a given square, -1 if empty
int Board::whichOccupySide(int file, int rank) {
	U64 result = 0;
	for (int i = 0; i < MAXPIECES; i++) {
		result = result | (Players[WHITE].Pieces[i] & square(file,rank)) ;
	}
	if (result >= 1) 
		return WHITE;
	result = 0;
	for (int i = 0; i < MAXPIECES; i++) {
		result = result | (Players[BLACK].Pieces[i] & square(file,rank));
	}
	if (result >= 1)
		return BLACK;
	return -1;
}

// returns the piece in a occupied sqaure
int Board::whichOccupyPiece(int file, int rank, int color) {
	int result = -1;
	for (int i=0; i<MAXPIECES; i++) {
		if ( (U64)(Players[color].Pieces[i] & square(file, rank))  != (U64)0){
			result = i;
			break;
		}
	}
	return result;
}

// prints the bitboards of a given piece
void Board::toBitboardString(int color, int piece) {
	U64 temp;
	for (int rank = 8; rank > 0; rank--) {
		for (int file = 1; file < 9; file++) {
			temp = (Players[color].Pieces[piece] & square(file, rank));
			if (temp >= 1)
				cout << 1 << " ";
			else
				cout << 0 << " ";

		}
		cout << endl;

	} 
	 cout << endl;
}

void Board::toBitboardString(U64 piece) {
	cout << "  a b c d e f g h" << endl;
	for (int rank = 8; rank > 0; rank--) {
		cout << rank <<" ";
		for (int file = 1; file < 9; file++) {
			if ((piece & square(file, rank) ) >= 1 )
				cout << 1 << " ";
			else
				cout << 0 << " ";

		}
		cout << endl;

	} 
	 cout << endl;

}

// moves the given piece in a given direction (N,E,S,W)
U64 Board::moveOne(U64 piece, int d) {
	if (d == NORTH) {
		return (~rank_i(8) & piece) << 8; 
	}
	else if (d == SOUTH) {
		return (~rank_i(1) & piece) >> 8;
	}
	else if (d == EAST) {
		return (~file_i(8) & piece) << 1;
	}
	else {
		return (~file_i(1) & piece) >> 1;
	}
	return 0;
}

U64 Board::whitePieces() {
	U64 result = 0;
	for (int i=0; i< MAXPIECES; i++) {
		result = result | Players[WHITE].Pieces[i];
	}
	return result;
}

U64 Board::blackPieces() {
	U64 result = 0;
	for (int i=0; i< MAXPIECES; i++) {
		result = result | Players[BLACK].Pieces[i];
	}
	return result;
}


U64 Board::empty() {
	return (U64) ~(whitePieces() | blackPieces()) ;
}

void Board::representBoard(int color){
	string s ="\n", f = "", rev = "\n";
	int sq, file, rank;
	char p='f';
	if (color == BLACK)
		cout << "  ";
	cout << "A B C D E F G H";
	for (rank = 1; rank <=8; rank++) {
		f ="";
		s += to_string(rank) + " ";
		for (file = 1; file<=8; file++) {

			sq = whichOccupySide(file,rank);
			if (sq == -1) {
				f += "* ";
			}
			else {
				switch(whichOccupyPiece(file, rank, sq)){
					case PAWN: p = (sq)?'p':'t';
							   break;
					case ROOK: p = 'r';
							   break;
					case KNIGHT: p = 'n';
								 break;
					case BISHOP: p = 'b';
								 break;
					case KING: p = 'k';
							   break;
					case QUEEN: p = 'q';
								break;
				}
				p = (sq == WHITE)? toupper(p):p;
				f += string(1,p) + " ";
				
			}
		}
		s += f + "\n";
		reverse(f.begin(), f.end());
		rev += to_string(rank)+ " ";
		rev += f + "\n";
	}
	reverse(rev.begin(), rev.end());
	if (color == WHITE) {
		cout << rev;
	}
	else
		cout << s;
	cout << endl;
}
