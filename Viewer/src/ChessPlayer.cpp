#include"ChessPlayer.h"



// initialize player by set his given time, Color and pieces
ChessPlayer::ChessPlayer(int time, bool whiteElseBlack) : timer(time), whiteElseBlack(whiteElseBlack), kingInThreat(false)
{
	pieces[0] = new King(whiteElseBlack);
	pieces[1] = new Queen(whiteElseBlack);
	pieces[2] = new Rook(whiteElseBlack);
	pieces[3] = new Rook(whiteElseBlack);
	pieces[4] = new Bishop(whiteElseBlack);
	pieces[5] = new Bishop(whiteElseBlack);
	pieces[6] = new Horse(whiteElseBlack);
	pieces[7] = new Horse(whiteElseBlack);
	for (int i = 8; i < NUM_OF_PIECES; i++)
	{
		pieces[i] = new Pawn(whiteElseBlack);
	}
}

// Free all the dynamic allocated pieces
ChessPlayer::~ChessPlayer()
{
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		delete pieces[i];
	}
}

// false - king is threatened. true - king is not threatened
bool ChessPlayer::GetKingInsThreat() const
{
	return kingInThreat;
}


// piece methods

Piece::Piece(bool whiteElseBlack) : whiteElseBlack(whiteElseBlack), pieceImage(NULL), pieceType(DefaultType)
{
	loacationOnBoard[0] = 0;
	loacationOnBoard[1] = 1;
}

Piece::~Piece()
{
	delete[] pieceImage;
}

PieceType Piece::getPieceType() const
{
	return pieceType;
}

Pawn::Pawn(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = PawnType;
}


void Pawn::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_Pawn.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_Pawn.png", &width, &height, &Channels, 0);
	}

}

Rook::Rook(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = RookType;
}

void Rook::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_Rook.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_Rook.png", &width, &height, &Channels, 0);
	}

}


Horse::Horse(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = HorseType;
}

void Horse::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_Horse.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_Horse.png", &width, &height, &Channels, 0);
	}

}

Bishop::Bishop(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = BishopType;
}

void Bishop::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_Bishop.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_Bishop.png", &width, &height, &Channels, 0);
	}

}

King::King(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = KingType;
}

void King::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_King.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_King.png", &width, &height, &Channels, 0);
	}

}


Queen::Queen(bool whiteElseBlack) : Piece(whiteElseBlack)
{
	pieceType = QueenType;
}

void Queen::InsertImage()
{
	int width, height, Channels;
	if (whiteElseBlack)
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/White_Queen.png", &width, &height, &Channels, 0);
	}
	else
	{
		unsigned char* pieceImage = stbi_load("../../Data/ChessImages/Black_Queen.png", &width, &height, &Channels, 0);
	}

}