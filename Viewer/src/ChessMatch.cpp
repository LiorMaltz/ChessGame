#include"ChessMatch.h"

ChessMatch::ChessMatch(ChessPlayer* whitePlayer, ChessPlayer* blackPlayer1, unsigned char* chessBoardImage) : turn(false), chessBoardImage(chessBoardImage), chessBoard()
{
	players[0] = whitePlayer;
	players[1] = blackPlayer1;
}

ChessMatch::~ChessMatch()
{
	delete[] chessBoardImage;
	delete[] players;

}

void ChessMatch::MakeMove(int* from, int* to)
{
	bool pieceMoveValid = false;
	// implement king is safe checker func
	bool kingISafe = players[turn]->GetKingIsSafe();
	
	// Find king coordinates
	int kingCoord[2];
	for (int i = 0; i < BOARDROWS; i++)
	{
		for (int j = 0; j < BOARDCOLS; j++)
		{
			if (chessBoard[i][j].piece->getPieceType() == KingType && chessBoard[i][j].blackElseWhiteIsOn == turn)
			{
				kingCoord[0] = i;
				kingCoord[1] = j;
			}
		}
	}
	ChessBoardCell* fromCell = &chessBoard[from[0]][from[1]];
	ChessBoardCell* toCell = &chessBoard[to[0]][to[1]];
	// Check if from is legel input to move from
	if ((turn == fromCell->blackElseWhiteIsOn) && fromCell->cellIsOccupied)
	{
		Piece* piece;
		
		// Based on piece type we will calculate the validation of the move
		switch (fromCell->piece->getPieceType())
		{
		case PawnType:
			// Step forward
			if (from[0] == to[0] && (to[1] - from[1]) == 1)
			{
				if (!toCell->cellIsOccupied)
				{
					pieceMoveValid = true;
				}
			}
			break;
		case BishopType:
			break;
		case HorseType:
			break;
		case RookType:
			break;
		case KingType:
			break;
		case QueenType:
			break;
		default:
			break;
		}
	}
}
