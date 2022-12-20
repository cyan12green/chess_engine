#include "board.h"
#include "eval.h"

class Engine {
	public: 
		static int minimax(Board position, int depth, bool maxPlayer, int color) {
			if ((depth == 0) || (position.isCheckMate())) {
				return Eval::evaluate(position);
			}
			
			int eval;

			if (maxPlayer) {
				int maxEval = INT_MIN;
				for (Board child: Board::legalMoves(color, position)) {
					eval = minimax(child, depth-1, false, 1-color);
					maxEval = (maxEval > eval)? maxEval:eval;
				}
				return maxEval;
			}
			else {
				int minEval = INT_MAX;
				for (Board child: Board::legalMoves(color, position)) {
					eval = minimax(child, depth-1, true, 1-color);
					minEval = (minEval < eval)? minEval:eval;
				}
				return minEval;
			}
		}
		
		static int minimaxAB(Board position, int depth, bool maxPlayer, int color, int alpha, int beta, vector<Board>* moves) {
			if ((depth == 0) || (position.isCheckMate())) {
				moves->push_back(position);
				return Eval::evaluate(position);
			}
			
			int eval;
			vector<Board> retMove, bestMove;
			if (maxPlayer) {
				int maxEval = INT_MIN;
				Board maxB;
				for (Board child: Board::legalMoves(color, position)) {
					retMove.clear();
					eval = minimaxAB(child, depth-1, false, 1-color, alpha, beta, &retMove);
					if (maxEval < eval) {
						maxEval = eval;
						maxB = child;
						bestMove = retMove;
					}
					
					alpha = (alpha > maxEval)? alpha:maxEval;
					if (beta <= maxEval)
						break;
				}
				bestMove.push_back(maxB);
				*moves = bestMove;
				return maxEval;
			}
			else {
				int minEval = INT_MAX;
				Board minB;
				for (Board child: Board::legalMoves(color, position)) {
					retMove.clear();
					eval = minimaxAB(child, depth-1, true, 1-color, alpha, beta, &retMove);
					if (minEval > eval) {
						minEval = eval;
						minB = child;
						bestMove = retMove;
					}
					
					beta = (beta < minEval)? beta:minEval;
					if (minEval <= alpha)
						break;
				}
				bestMove.push_back(minB);
				*moves = bestMove;
				return minEval;
			}
		}
		
};


