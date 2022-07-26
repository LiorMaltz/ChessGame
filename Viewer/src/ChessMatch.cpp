#include"ChessMatch.h"

// Threat function declerations
bool PawnThreat(const int* threatLocation, const int* kingLocation, const Turn& turn);
bool HorseThreat(const int* threatLocation, const int* kingLocation);
bool BishopThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation);
bool RookThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation);
bool QueenThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation);
bool KingThreat(const int* threatLocation, const int* kingLocation);

// move ligality examination declerations
bool PawnMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn, const bool& firstMove);
bool BishopMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn);
bool HorseMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn);
bool RookMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn);
bool KingMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn);
bool QueenMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn);


// When initialzing the game we decide the first turn should be of white, and we should  
ChessMatch::ChessMatch(const int& time) : turn(WHITE)
{
	// Gives chess match image
	chessBoardImage = "../Data/ChessImages/ChessBoard.png";

	// Initialize the two opponents
	players[0] = new ChessPlayer(time, WHITE);
	players[1] = new ChessPlayer(time, BLACK);


	// White placement on board
	// not pawn placement
	for (int i = 0; i < NUM_OF_PIECES / 2; i++)
	{
		chessBoard[i][0].blackElseWhiteIsOn = WHITE;
		chessBoard[i][0].cellIsOccupied = true;
	}
	chessBoard[0][0].piece = players[WHITE]->pieces[2];
	chessBoard[1][0].piece = players[WHITE]->pieces[6];
	chessBoard[2][0].piece = players[WHITE]->pieces[4];
	chessBoard[3][0].piece = players[WHITE]->pieces[1];
	chessBoard[4][0].piece = players[WHITE]->pieces[0];
	chessBoard[5][0].piece = players[WHITE]->pieces[5];
	chessBoard[6][0].piece = players[WHITE]->pieces[7];
	chessBoard[7][0].piece = players[WHITE]->pieces[3];



	// Pawn placement
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		chessBoard[i][1].piece = players[WHITE]->pieces[i + 8];
		chessBoard[i][1].blackElseWhiteIsOn = WHITE;
		chessBoard[i][1].cellIsOccupied = true;
	}

	// Black placement on board
	// not pawn placement
	for (int i = 0; i < NUM_OF_PIECES / 2; i++)
	{
		chessBoard[i][7].blackElseWhiteIsOn = BLACK;
		chessBoard[i][7].cellIsOccupied = true;
	}

	chessBoard[0][7].piece = players[BLACK]->pieces[2];
	chessBoard[1][7].piece = players[BLACK]->pieces[6];
	chessBoard[2][7].piece = players[BLACK]->pieces[4];
	chessBoard[3][7].piece = players[BLACK]->pieces[1];
	chessBoard[4][7].piece = players[BLACK]->pieces[0];
	chessBoard[5][7].piece = players[BLACK]->pieces[5];
	chessBoard[6][7].piece = players[BLACK]->pieces[7];
	chessBoard[7][7].piece = players[BLACK]->pieces[3];

	// Pawn placement
	for (int i = 0; i < NUM_OF_PIECES / 2; i++)
	{
		chessBoard[i][6].piece = players[BLACK]->pieces[i + 8];
		chessBoard[i][6].blackElseWhiteIsOn = BLACK;
		chessBoard[i][6].cellIsOccupied = true;
	}

	// Update pices positions
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			if (chessBoard[i][j].piece != NULL)
			{
				chessBoard[i][j].piece->locationOnBoard[0] = i;
				chessBoard[i][j].piece->locationOnBoard[1] = j;
			}
		}
	}

	
	ConstructChessBoardBuffer();

}

ChessMatch::~ChessMatch()
{
	delete players[0];
	delete players[1];

}

// returns: 1 - there is a mate, 2 - there is a draw, false - game can continue
int ChessMatch::MateOrDraw()
{
	bool kingInThreat = CheckKingThreat();
	int toLocation[2];

	// for every piece we will check if there is any squere it can legaly move
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		int pieceLocation[2] = { players[turn]->pieces[0]->locationOnBoard[0], players[turn]->pieces[0]->locationOnBoard[1] };
		for (int i = 0; i < BOARD_ROWS; i++)
		{
			for (int j = 0; j < BOARD_COLS; j++)
			{
				toLocation[0] = i;
				toLocation[1] = j;
				if (TestMove(pieceLocation, toLocation))
				{
					// return that the game can continue
					return 0;
				}
			}
		}
	}
	// No room to move and there is a theat
	if (kingInThreat)
	{
		// return mate
		return 1;
	}
	// No room to move and there is no a theat
	else
	{
		// return draw
		return 2;
	}
}

// If the move was executed return true, else return false
bool ChessMatch::Move(const int* from, const int* to)
{
	if (TestMove(from, to))
	{
		MakeMove(from, to);
		return true;
	}
	return false;
}

// Function to move the pieces on the board. will recieve two int[2] arrays that represet 'from' coordintes and 'to' coordintes on the chess table
void ChessMatch::MakeMove(const int* from, const int* to)
{
	ChessBoardCell* fromCell = &chessBoard[from[0]][from[1]];
	ChessBoardCell* toCell = &chessBoard[to[0]][to[1]];
	
	// toCell - blackElseWhiteIsOn done, cellIsOccupied done, piece ptr done (delete, null in player, and set it from fromCell).
	// fromCell - blackElseWhiteIsOn no need, cellIsOccupied done, piece ptr done (set location and null it).

	// dealing with the eaten piece
	if (toCell->cellIsOccupied)
	{
		toCell->piece->model->SetWorldLocation(vec3(-1,-1,-1));
		int kiaPieceID = toCell->piece->GetID();
		delete players[!turn]->pieces[kiaPieceID];
		players[!turn]->pieces[kiaPieceID] = NULL;
	}

	toCell->piece = fromCell->piece;
	fromCell->piece = NULL;
	toCell->piece->SetLocation(to);
	toCell->blackElseWhiteIsOn = turn;
	toCell->cellIsOccupied = true;
	fromCell->cellIsOccupied = false;

	// update location in the piece object
	int currntPieceID = toCell->piece->GetID();
	players[turn]->pieces[currntPieceID]->locationOnBoard[0] = to[0];
	players[turn]->pieces[currntPieceID]->locationOnBoard[1] = to[1];

	// move the piece - graphic part
	toCell->piece->model->TranslateObject(vec3(stepDistOnForSquere * (to[0] - from[0]), stepDistOnForSquere * (to[1] - from[1]), 0), true);

	// Change turn
	if (turn == WHITE)
		turn = BLACK;
	else
		turn = WHITE;

	

}

// return true - the move is legal, return false the move is not legal
bool ChessMatch::TestMove(const int* from, const int* to)
{
	bool pieceMoveValid = true;
	// implement king is safe checker func
	bool kingISafe = players[turn]->GetKingInThreat();
	
	ChessBoardCell* fromCell = &chessBoard[from[0]][from[1]];
	ChessBoardCell* toCell = &chessBoard[to[0]][to[1]];

	// Find king coordinates
	int kingCoord[2] = { players[turn]->pieces[0]->locationOnBoard[0], players[turn]->pieces[0]->locationOnBoard[1] };

	// Can't move to the same squere
	if (from[0] == to[0] && from[1] == to[1])
	{
		pieceMoveValid = false;
	}
	// Check if from is legel input to move from
	else if ((turn == fromCell->blackElseWhiteIsOn) && fromCell->cellIsOccupied)
	{
		switch (fromCell->piece->GetPieceType())
		{
		case PieceType::PawnType:
			pieceMoveValid = PawnMove(chessBoard, from, to, turn, dynamic_cast<Pawn*>(fromCell->piece)->firstMoove);
			break;
		case PieceType::BishopType:
			pieceMoveValid = BishopMove(chessBoard, from, to, turn);
			break;
		case PieceType::HorseType:
			pieceMoveValid = HorseMove(chessBoard, from, to, turn);
			break;
		case PieceType::RookType:
			pieceMoveValid = RookMove(chessBoard, from, to, turn);
			break;
		case PieceType::KingType:
			pieceMoveValid = KingMove(chessBoard, from, to, turn);
			break;
		case PieceType::QueenType:
			pieceMoveValid = QueenMove(chessBoard, from, to, turn);
			break;
		default:
			pieceMoveValid = false;
		}
	}
	// The from cell contains empty pointer. means it is not contains a piece
	else
		pieceMoveValid = false;
	
	// If the move seemed legal without king threat calculation we will now consider the king
	if (pieceMoveValid)
	{

		// Execute preempting measures that could tell us if the king in threat after the move
		// If the king is in threat we will restore the match state and cancel the move

		bool tempColor = toCell->blackElseWhiteIsOn;
		bool tempOccupied = toCell->cellIsOccupied;
		if (fromCell->piece->GetPieceType() == PieceType::KingType)
		{
			players[turn]->pieces[0]->locationOnBoard[0] = to[0];
			players[turn]->pieces[0]->locationOnBoard[1] = to[1];
		}

		fromCell->cellIsOccupied = false;
		toCell->cellIsOccupied = true;
		toCell->blackElseWhiteIsOn = turn;

		bool KingInThreat = CheckKingThreat();

		fromCell->cellIsOccupied = true;
		toCell->blackElseWhiteIsOn = tempColor;
		toCell->cellIsOccupied = tempOccupied;
		if (fromCell->piece->GetPieceType() == PieceType::KingType)
		{
			players[turn]->pieces[0]->locationOnBoard[0] = from[0];
			players[turn]->pieces[0]->locationOnBoard[1] = from[1];
		}


		// Valid move
		if (!KingInThreat)
		{
			return true;
		}
	}
	// return move is not valid
	return false;
}

// true - check on king. false - no check
bool ChessMatch::CheckKingThreat()
{
	int threatLocation[2];
	// extract the king's player with the turn
	int kingLocation[2] = { players[turn]->pieces[0]->locationOnBoard[0] , players[turn]->pieces[0]->locationOnBoard[1] };
	ChessBoardCell** a = NULL;
	//a = chessBoard;
	// Will see if there is a threat from any square. (can change the loop from squares to pieces with iterators)
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			// Check for a possible threat from opponent piece
			if (chessBoard[i][j].cellIsOccupied && chessBoard[i][j].blackElseWhiteIsOn != turn)
			{
				threatLocation[0] = i; threatLocation[1] = j;
				switch (chessBoard[i][j].piece->GetPieceType())
				{
				case PieceType::PawnType:
					if (PawnThreat(threatLocation, kingLocation, turn))
					{
						return true;
					}
					break;
				case PieceType::BishopType:
					if (BishopThreat(chessBoard, threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case PieceType::HorseType:
					if (HorseThreat(threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case PieceType::RookType:
					if (RookThreat(chessBoard, threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case PieceType::KingType:
					if (KingThreat(threatLocation, kingLocation))
					{
						return true;
					}
					break;
				case PieceType::QueenType:
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
	if (abs(threatLocation[0] - kingLocation[0]) == 2)
	{
		if (abs(threatLocation[1] - kingLocation[1]) == 1)
		{
			return true;
		}
	}

	// Examine the other 4 ways possible checks
	if (abs(threatLocation[1] - kingLocation[1] ) == 2)
	{
		if (abs(threatLocation[0] - kingLocation[0]) == 1)
		{
			return true;
		}
	}
	return false;

}

bool BishopThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation)
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

			while (threatLocation[0] < BOARD_ROWS && threatLocation[1] < BOARD_COLS)
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

			while (threatLocation[0] >= 0 && threatLocation[1] < BOARD_COLS)
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

			while (threatLocation[0] < BOARD_ROWS && threatLocation[1] >= 0)
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
	return false;
}

bool RookThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation)
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

			while (threatLocation[1] < BOARD_COLS)
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

			while (threatLocation[0] < BOARD_ROWS)
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

bool QueenThreat(const ChessBoardCell chessBoard[][BOARD_COLS], const int* threatLocation, const int* kingLocation)
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

bool PawnMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn, const bool& firstMove)
{
	if (from[0] == to[0] && (((to[1] - from[1]) == 1 || ((to[1] - from[1]) == 2 && firstMove) && !turn) || ((to[1] - from[1]) == -1 || ((to[1] - from[1]) == -2 && firstMove) && turn)))
	{
		if (!chessBoard[to[0]][to[1]].cellIsOccupied)
		{
			return true;
		}
	}
	return false;
}

bool BishopMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn)
{
	// Check diagonal
	if (abs(from[0] - to[0]) == abs(from[1] - to[1]))
	{
		// Parameters that direct the direction we will we move to check the squeres
		int xPar = 1, yPar = 1;
		int walker[2] = { from[0] , from[1] };
		if (from[0] > to[0]) xPar *= -1;
		if (from[1] > to[1]) yPar *= -1;
		walker[0] += xPar; walker[1] += yPar;


		// check for obsitcales Until we come to 'to' squere
		while (walker[0] != to[0] && walker[1] != to[1] )
		{
			if (chessBoard[walker[0]][walker[1]].cellIsOccupied)
			{
				return false;
			}
			walker[0] += xPar; walker[1] += yPar;
		}
		// Include 'to' sqeure
		if ((chessBoard[walker[0]][walker[1]].cellIsOccupied && chessBoard[walker[0]][walker[1]].blackElseWhiteIsOn == turn) || (walker[0] != to[0] && walker[1] != to[0]))
		{
			return false;
		}
	}
	// If 'to' not diagonal to 'from' bishop can't make the move
	else
		return false;
	return true;
	
}

bool HorseMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn)
{
	// If to squere is not one of the 8 valid candidates OR to squere is occupied the move is not valid
	if (!((abs(from[0] - to[0]) == 2 && abs(from[1] - to[1]) == 1) || (abs(from[1] - to[1]) == 2 && abs(from[0] - to[0]) == 1)) || chessBoard[to[0]][to[1]].cellIsOccupied && chessBoard[to[0]][to[1]].blackElseWhiteIsOn == turn)
	{
		return false;
	}

	return true;

}

bool RookMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn)
{
	// same x number
	if (from[0] == to[0])
	{
		int yPar = 1;
		if (from[1] > to[1])
			yPar = -yPar;
		int walker = from[1] + yPar;
		while (walker != to[1])
		{
			if (chessBoard[from[0]][walker].cellIsOccupied)
			{
				return false;
			}
			walker += yPar;
		}
		if ((chessBoard[from[0]][walker].cellIsOccupied && chessBoard[from[0]][walker].blackElseWhiteIsOn == turn) || walker != to[1] )
		{
			return false;
		}

	}
	// same y axis
	else if (from[1] == to[1])
	{
		int xPar = 1;
		if (from[0] > to[0])
			xPar = -xPar;
		int walker = from[0] + xPar;
		while (walker != to[0])
		{
			if (chessBoard[walker][from[1]].cellIsOccupied)
			{
				return false;
			}
			walker += xPar;
		}
		if ((chessBoard[walker][from[1]].cellIsOccupied && chessBoard[walker][from[1]].blackElseWhiteIsOn == turn) || walker != to[0])
		{
			return false;
		}

	}
	// not in a same axis
	else
		return false;

	return true;
}

bool KingMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn)
{
	// Or to squere is occupied, or in not a reachable squere to the king
	if (!(abs(from[0] - to[0]) <= 1 && abs(from[1] - to[1]) <= 1) || (chessBoard[to[0]][to[1]].cellIsOccupied && turn == chessBoard[to[0]][to[1]].blackElseWhiteIsOn))
	{
		return false;
	}
	return true;
}

bool QueenMove(const ChessBoardCell chessBoard[][BOARD_COLS], const int* from, const int* to, const Turn& turn)
{
	// Just if one of the sub function is correct so the move is legal
	return (RookMove(chessBoard, from, to, turn) || BishopMove(chessBoard, from, to, turn));
}


// Get/Set

ChessPlayer* ChessMatch::GetPlayer(const int& num)
{
	return players[num];
}

TableColorType* ChessMatch::GetTableColorType()
{
	return &chessBoardColorData;
}


// Buffer update 
void ChessMatch::ConstructChessBoardBuffer()
{

	tableColorBindingPlace = 3;
	glGenBuffers(1, &tableColorBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tableColorBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * 8 * 8, chessBoardColorData.data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, tableColorBindingPlace, tableColorBuffer);
	chessBoardColorData.change = false;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}