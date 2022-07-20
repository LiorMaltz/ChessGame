#ifndef CHESS_PLAYER_CLASS_H
#define CHESS_PLAYER_CLASS_H
#include<chrono>
#include "stb_image.h"

using namespace std::chrono;

#define NUM_OF_PIECES 16

enum PieceType { KingType, QueenType, RookType, HorseType, BishopType, PawnType, DefaultType };

// Making abstact class for all the chess pieces
class Piece
{
public:
	unsigned char* pieceImage;
	PieceType pieceType;
	bool whiteElseBlack;
	Piece(bool whiteElseBlack);
	PieceType getPieceType() const;

	void virtual InsertImage() = 0;
	virtual ~Piece();
private:

};

Piece::Piece(bool whiteElseBlack) : whiteElseBlack(whiteElseBlack), pieceImage(NULL), pieceType(DefaultType) {}

Piece::~Piece()
{
	delete[] pieceImage;
}

PieceType Piece::getPieceType() const
{
	return pieceType;
}

class Pawn : public Piece
{
public:
	Pawn(bool whiteElseBlack);
	void virtual InsertImage();
};

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

class Rook : public Piece
{
public:
	Rook::Rook(bool whiteElseBlack);
	void virtual InsertImage();
};

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

class Horse : public Piece
{
public:
	Horse::Horse(bool whiteElseBlack);
	void virtual InsertImage();
};

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

class Bishop : public Piece
{
public:
	Bishop::Bishop(bool whiteElseBlack);
	void virtual InsertImage();
};

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

class King : public Piece
{
public:
	King::King(bool whiteElseBlack);
	void virtual InsertImage();
};

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

class Queen : public Piece
{
public:
	Queen::Queen(bool whiteElseBlack);
	void virtual InsertImage();
};

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


class ChessPlayer
{
public:
	bool kingIsSafe;
	bool whiteElseBlack;
	milliseconds timer;
	Piece* pieces[16];

	ChessPlayer(int time, bool whiteElseBlack);
	~ChessPlayer();

	bool GetKingIsSafe() const;

private:

};

// initialize player by set his given time, Color and pieces
ChessPlayer::ChessPlayer(int time, bool whiteElseBlack) : timer(time), whiteElseBlack(whiteElseBlack) , kingIsSafe(true)
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

bool ChessPlayer::GetKingIsSafe() const
{
	return kingIsSafe;
}

#endif // !CHESS_PLAYER_CLASS_H
