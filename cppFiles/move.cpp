#include "board.h"
//TODO enfore enpassent
U64 Board::getMoveSetOfPiece(int piece, int color, Board b) {
	U64 ret = 0ULL, tempPiece = b.Players[color].Pieces[piece], friendly;
	friendly = (color == BLACK)?b.blackPieces():b.whitePieces();
	switch(piece) {
		case PAWN: 
			ret |= b.PawnSinglePushTargets(tempPiece, color); 
			ret |= b.PawnDoublePushTargets(tempPiece, color);
			ret |= b.PawnsAnyAttacks(tempPiece, color); 
			break;

		case KNIGHT:
			while (tempPiece != 0ULL) {
				ret |= KnightAttacksLookup[pop_1st_bit(&tempPiece)];
			}
			break;

		case KING:
			ret |= KingAttacksLookup[pop_1st_bit(&tempPiece)];
			break;

		default: // rook, bishop and queen
			while (tempPiece != 0ULL) {
				ret |= getMovesSlidingPiece(piece, pop_1st_bit(&tempPiece), b.whitePieces() | b.blackPieces()); 
			}
	}
	return ret & ~friendly;   
}

bool Board::isSqAttackedBy(int sq, int color, Board *b) {
	U64 ret = 0ULL;
	for (int i=0; i<MAXPIECES; i++) {
		ret |= getMoveSetOfPiece(i, color, (Board)*b);
	}
	return (((U64)1 << sq) & ret) != (U64)0;
}
Board Board::capturing(int piece, int color, U64 bitboard, Board b) {
	while (bitboard) {
		U64 temp = ONE << (pop_1st_bit(&bitboard));
		for (int i = 0; i< MAXPIECES; i++) {
			if (b.Players[1-color].Pieces[i] & temp) {
				b.Players[1-color].Pieces[i] ^= temp;
				b.halfmove = 0;
				break;
			}
		}
	}

	return b;
}

vector<Board> Board::legalMoves(int color, Board b){
	vector<Board> ret;
	Board tempBoard;
	int ind;
	U64 tempMoves, tempPiece1, tempPiece2, friendly;
	friendly = (color == WHITE)?b.whitePieces():b.blackPieces();

	for (int i=0; i<MAXPIECES; i++) {
		switch(i){
			case PAWN: 
				tempMoves = b.Players[color].Pieces[PAWN];
				while(tempMoves) {
					tempPiece1 = ONE << (pop_1st_bit(&tempMoves)); //pawns
					tempPiece2 = (b.PawnSinglePushTargets(tempPiece1, color)) & ~friendly; 
					while (tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[PAWN] ^= tempPiece1;
						tempBoard.Players[color].Pieces[PAWN] |= ONE << pop_1st_bit(&tempPiece2);
						tempBoard.halfmove = 0;
						if (!tempBoard.isInCheck(color))
							ret.push_back(tempBoard);
					}
					tempPiece2 = (b.PawnDoublePushTargets(tempPiece1, color)) & ~friendly;
					while(tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[PAWN] ^= tempPiece1;
						U64 forEnP = ONE << pop_1st_bit(&tempPiece2);
						tempBoard.Players[color].Pieces[PAWN] |= forEnP;
						if (color == WHITE) {
							tempBoard.enPassent = moveOne(forEnP, SOUTH);
						}
						else {
							tempBoard.enPassent = moveOne(forEnP, NORTH);
						}
						tempBoard.halfmove = 0;
						if (!tempBoard.isInCheck(color))
							ret.push_back(tempBoard);
					}
					tempPiece2 = b.PawnsAnyAttacks(tempPiece1, color) & ~friendly;
					while(tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[PAWN] ^= tempPiece1;
						tempBoard = capturing(PAWN, color, ONE << pop_1st_bit(&tempPiece2), tempBoard);
						if (!tempBoard.isInCheck(color))
							ret.push_back(tempBoard);
					}
				}
				break;
			case KNIGHT: 
				tempMoves = b.Players[color].Pieces[KNIGHT];
				while(tempMoves) {
					ind = pop_1st_bit(&tempMoves);
					tempPiece1 = ONE << ind;
					tempPiece2 = KnightAttacksLookup[ind];
					while(tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[KNIGHT] ^= tempPiece1;
						ind = pop_1st_bit(&tempPiece2);
						if ((ONE << ind) & ~friendly) {
							tempBoard.Players[color].Pieces[KNIGHT] |= (ONE << ind);
							tempBoard.halfmove++;
							tempBoard = capturing(KNIGHT, color, (ONE << ind), tempBoard);
							
							if (!tempBoard.isInCheck(color))
								ret.push_back(tempBoard);
						}
					}
				}
			 	break;
			case KING: 
				tempMoves = b.Players[color].Pieces[KING];
				while(tempMoves) {
					ind = pop_1st_bit(&tempMoves);
					tempPiece1 = ONE << ind;
					tempPiece2 = KingAttacksLookup[ind];
					while(tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[KING] ^= tempPiece1;
						ind = pop_1st_bit(&tempPiece2);
						if ((ONE << ind) & ~friendly) {
							tempBoard.Players[color].Pieces[KING] |= (ONE << ind);
							tempBoard.halfmove++;
							tempBoard = capturing(KING, color, (ONE << ind), tempBoard);
							if (!tempBoard.isInCheck(color))
								ret.push_back(tempBoard);
						}
					}
				}
				if (b.canCastleKing(color)) {
					tempBoard = b;
					tempBoard.castleKingSide(color);
					tempBoard.halfmove++;
					ret.push_back(tempBoard);
				}
				if (b.canCastleQueen(color)) {
					tempBoard = b;
					tempBoard.castleQueenSide(color);
					tempBoard.halfmove++;
					ret.push_back(tempBoard);
				}
				break;
			default:
				tempMoves = b.Players[color].Pieces[i];
				while(tempMoves) {
					ind = pop_1st_bit(&tempMoves);
					tempPiece1 = ONE << ind;
					tempPiece2 = getMovesSlidingPiece(i,ind, b.whitePieces() | b.blackPieces());
					while(tempPiece2) {
						tempBoard = b;
						tempBoard.Players[color].Pieces[i] ^= tempPiece1;
						ind = pop_1st_bit(&tempPiece2);
						if ((ONE << ind) & ~friendly) {
							tempBoard.Players[color].Pieces[i] |= (ONE << ind);
							tempBoard.halfmove++;
							tempBoard = capturing(i, color, (ONE << ind), tempBoard);
							if (!tempBoard.isInCheck(color))
								ret.push_back(tempBoard);
						}
					}
				}
			 	break;
			
		}
	}
	if (color == BLACK) {
		for (Board c : ret) {
			c.fullmoveCounter++;
		}
	}
	return ret;
}
