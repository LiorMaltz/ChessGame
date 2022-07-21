#include"ChessMatch.h"

// Threat function declerations
bool PawnThreat(const int* threatLocation, const int* kingLocation, const Turn& turn);
bool HorseThreat(const int* threatLocation, const int* kingLocation);
bool BishopThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation);
bool RookThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation);
bool QueenThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation);
bool KingThreat(const int* threatLocation, const int* kingLocation);

// move ligality examination declerations
bool PawnMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to, const Turn& turn);
bool BishopMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to);
bool HorseMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to);
bool RookMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to);
bool KingMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to);
bool QueenMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to);


ChessMatch::ChessMatch(ChessPlayer* whitePlayer, ChessPlayer* blackPlayer1, unsigned char* chessBoardImage) : turn(WHITE), chessBoardImage(chessBoardImage), chessBoard()
{
	players[0] = whitePlayer;
	players[1] = blackPlayer1;
}

ChessMatch::~ChessMatch()
{
	delete[] chessBoardImage;

	delete players[0];
	delete players[1];

}

void ChessMatch::MakeMove(int* from, int* to)
{
	bool pieceMoveValid = true;
	// implement king is safe checker func
	bool kingISafe = players[turn]->GetKingInsThreat();
	
	ChessBoardCell* fromCell = &chessBoard[from[0]][from[1]];
	ChessBoardCell* toCell = &chessBoard[to[0]][to[1]];

	// Find king coordinates
	int kingCoord[2] = { players[turn]->pieces[0]->loacationOnBoard[0], players[turn]->pieces[0]->loacationOnBoard[1] };

	// Can't move to the same squere
	if (from[0] == to[0] && from[1] == to[1])
	{
		pieceMoveValid = false;
	}
	// Check if from is legel input to move from
	else if ((turn == fromCell->blackElseWhiteIsOn) && fromCell->cellIsOccupied)
	{
		Piece* piece;
		
		// Based on piece type we will calculate the validation of the move
		switch (fromCell->piece->getPieceType())
		{
		case PawnType:
			pieceMoveValid = PawnMove(chessBoard, from, to, turn);
			break;
		case BishopType:
			pieceMoveValid = BishopMove(chessBoard, from, to);
			break;
		case HorseType:
			pieceMoveValid = HorseMove(chessBoard, from, to);
			break;
		case RookType:
			pieceMoveValid = RookMove(chessBoard, from, to);
			break;
		case KingType:
			pieceMoveValid = KingMove(chessBoard, from, to);
			break;
		case QueenType:
			pieceMoveValid = QueenMove(chessBoard, from, to);
			break;
		}
	}

	// If the move seemed legal without king threat calculation we will now consider the king
	if (pieceMoveValid)
	{

		// Execute preempting measures that could tell us if the king in threat after the move
		// If the king is in threat we will restore the match state and cancel the move
		fromCell->cellIsOccupied = false;
		toCell->blackElseWhiteIsOn = turn;

	}
}


bool ChessMatch::CheckKingThreat()
{
	int threatLocation[2];
	// extract the king's player with the turn
	int kingLocation[2] = { players[turn]->pieces[0]->loacationOnBoard[0] , players[turn]->pieces[0]->loacationOnBoard[1] };
	ChessBoardCell** a = NULL;
	//a = chessBoard;
	// Will see if there is a threat from any square. (can change the loop from squares to pieces with iterators)
	for (int i = 0; i < BOARDROWS; i++)
	{
		for (int j = 0; j < BOARDCOLS; j++)
		{
			// Check for a possible threat from opponent piece
			if (chessBoard[i][j].cellIsOccupied && chessBoard[i][j].blackElseWhiteIsOn != turn)
			{
				threatLocation[0] = i; threatLocation[1] = j;
				switch (chessBoard[i][j].piece->getPieceType())
				{
				case PawnType:
					if (PawnThreat(threatLocation, kingLocation, turn))
					{
						return true;
					}
					break;
				case BishopType:
					if (BishopThreat(chessBoard, threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case HorseType:
					if (HorseThreat(threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case RookType:
					if (RookThreat(chessBoard, threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case KingType:
					if (KingThreat(threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case QueenType:
					if (QueenThreat(chessBoard, threatLocation, kingLocation))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}



// Threat function defenition

bool PawnThreat(const int* threatLocation, const int* kingLocation, const Turn& turn) 
{
	if (abs(threatLocation[1] - kingLocation[1]) == 1)
	{
		if (turn == WHITE && (threatLocation[0] - kingLocation[0] == 1))
			return true;
		if (turn == BLACK && (threatLocation[0] - kingLocation[0] == -1))
			return true;
	}
	return false;
}

bool HorseThreat(const int* threatLocation, const int* kingLocation)
{
	// Total of 8 possible ways of threats to check
	
	// Examine 4 ways of check here
	if (abs(threatLocation[0] - kingLocation[0] == 2))
	{
		if (abs(threatLocation[1] - kingLocation[1]) == 1)
		{
			return true;
		}
	}

	// Examine the other 4 ways possible checks
	if (abs(threatLocation[1] - kingLocation[1] == 2))
	{
		if (abs(threatLocation[0] - kingLocation[0]) == 1)
		{
			return true;
		}
	}
	return false;

}

bool BishopThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation)
{
	// up-right and down-left checks will pass the condition
	if (threatLocation[0] - kingLocation[0] == threatLocation[1] - kingLocation[1])
	{
		// Bishop is up-right to the king
		if (threatLocation[0] - kingLocation[0] > 0)
		{
			int threatToCheck[2] = { threatLocation[0] - 1, threatLocation[1] - 1 };

			while (threatLocation[0] >= 0 && threatLocation[1] >= 0)
			{
				if (threatToCheck[0] == kingLocation[0] && threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[0]--;
				threatToCheck[1]--;
			}

		}
		// Bishop is down-left to the king
		else
		{
			int threatToCheck[2] = { threatLocation[0] + 1, threatLocation[1] + 1 };

			while (threatLocation[0] < BOARDROWS && threatLocation[1] < BOARDCOLS)
			{
				if (threatToCheck[0] == kingLocation[0] && threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[0]++;
				threatToCheck[1]++;
			}
		}
	}

	// up-left and down-rght checks will pass the condition
	else if (threatLocation[0] - kingLocation[0] == -(threatLocation[1] - kingLocation[1]))
	{
		// up-left possible check
		if (threatLocation[0] - kingLocation[0] > 0)
		{
			int threatToCheck[2] = { threatLocation[0] - 1, threatLocation[1] + 1 };

			while (threatLocation[0] >= 0 && threatLocation[1] < BOARDCOLS)
			{
				if (threatToCheck[0] == kingLocation[0] && threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[0]--;
				threatToCheck[1]++;
			}
		}
		// down-right possible check
		else
		{
			int threatToCheck[2] = { threatLocation[0] + 1, threatLocation[1] - 1 };

			while (threatLocation[0] < BOARDROWS && threatLocation[1] >= 0)
			{
				if (threatToCheck[0] == kingLocation[0] && threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[0]++;
				threatToCheck[1]--;
			}
		}
	}
}

bool RookThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation)
{
	// on the same row
	if (threatLocation[0] == kingLocation[0])
	{
		// Rook is right to the king
		if (threatLocation[1] > kingLocation[1])
		{
			int threatToCheck[2] = { threatLocation[0], threatLocation[1] - 1 };

			while (threatLocation[1] >= 0)
			{
				if (threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[1]--;
			}
		}
		// Rook is left to the king
		else
		{
			int threatToCheck[2] = { threatLocation[0], threatLocation[1] + 1 };

			while (threatLocation[1] < BOARDCOLS)
			{
				if (threatToCheck[1] == kingLocation[1])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[1]++;
			}
		}
	}
	// on the same col
	if (threatLocation[1] == kingLocation[1])
	{
		// Rook is up to the king
		if (threatLocation[0] > kingLocation[0])
		{
			int threatToCheck[2] = { threatLocation[0] - 1, threatLocation[1] };

			while (threatLocation[0] >= 0)
			{
				if (threatToCheck[0] == kingLocation[0])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[0]--;
			}
		}
		// Rook is down to the king
		else
		{
			int threatToCheck[2] = { threatLocation[0] + 1, threatLocation[1] };

			while (threatLocation[0] < BOARDROWS)
			{
				if (threatToCheck[0] == kingLocation[0])
				{
					return true;
				}
				// If a piece is between the bishop and the king there is no check
				if (chessBoard[threatToCheck[0]][threatToCheck[1]].cellIsOccupied)
				{
					break;
				}
				threatToCheck[1]++;
			}
		}
	}
	return false;
}

bool QueenThreat(const ChessBoardCell chessBoard[][BOARDCOLS], const int* threatLocation, const int* kingLocation)
{
	if (RookThreat(chessBoard, threatLocation, kingLocation) || BishopThreat(chessBoard, threatLocation, kingLocation))
	{
		return true;
	}
	return false;
}

bool KingThreat(const int* threatLocation, const int* kingLocation)
{
	if ((abs(threatLocation[0] - kingLocation[0]) <= 1) && (abs(threatLocation[1] - kingLocation[1]) <= 1))
	{
		return true;
	}
	return false;
}


// move legality examination functions

bool PawnMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to, const Turn& turn)
{
	if (from[0] == to[0] && (((to[1] - from[1]) == 1 && !turn) || ((to[1] - from[1]) == -1 && turn)))
	{
		if (chessBoard[from[0]][from[1]].cellIsOccupied)
		{
			return false;
		}
	}
	return true;
}

bool BishopMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to)
{
	// Check diagonal
	if (abs(from[0] - to[0]) == abs(from[1] - to[1]))
	{
		// Parameters that direct the direction we will check the squeres
		int xPar = 1, yPar = 1;
		int walker[2] = { from[0] , from[1] };
		if (from[0] > to[0]) xPar *= -1;
		if (from[1] > to[1]) yPar *= -1;
		while (walker[0] < BOARDROWS && walker[1] >= 0 && walker[0] < BOARDCOLS && walker[1] >= 0)
		{
			if (chessBoard[walker[0]][walker[1]].cellIsOccupied)
			{
				return false;
				break;
			}
			walker[0] += xPar; walker[1] += yPar;
		}
	}
	return true;
}

bool HorseMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to)
{
	// If to squere is not one of the 8 valid candidates OR to squere is occupied the move is not valid
	if (!(((abs(from[0] - to[0]) == 2 && abs(from[1] - to[1])) == 1) || ((abs(from[1] - to[1]) == 2 && abs(from[0] - to[0]) == 1))) || chessBoard[from[0]][from[1]].cellIsOccupied)
	{
		return false;
	}

	return true;

}

bool RookMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to)
{
	if (from[0] == to[0])
	{
		int walker = from[1];
		int yPar = 1;
		if (from[1] > to[1]) yPar *= -1;
		while (walker < BOARDCOLS && walker >= 0)
		{
			if (chessBoard[from[0]][walker].cellIsOccupied)
			{
				return false;
				break;
			}
			walker += yPar;
		}
	}
	else if (from[1] == to[1])
	{
		int walker = from[0];
		int xPar = 1;
		if (from[0] > to[0]) xPar *= -1;
		while (walker < BOARDCOLS && walker >= 0)
		{
			if (chessBoard[walker][from[1]].cellIsOccupied)
			{
				return false;
				break;
			}
			walker += xPar;
		}
	}
	return true;
}

bool KingMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to)
{
	// Or to squere is occupied, or in not a reachable squere to the king
	if (chessBoard[from[0]][from[1]].cellIsOccupied || !(abs(from[0] - to[0]) <= 1 && abs(from[1] - to[1]) <= 1))
	{
		return false;
	}
	return true;
}

bool QueenMove(const ChessBoardCell chessBoard[][BOARDCOLS], const int* from, const int* to)
{
	// Just if one of the sub function is correct so the move is legal
	return (RookMove(chessBoard, from, to) || BishopMove(chessBoard, from, to));
}