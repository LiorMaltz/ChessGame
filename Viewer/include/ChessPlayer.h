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
	int loacationOnBoard[2];
	Piece(bool whiteElseBlack);
	PieceType getPieceType() const;

	void virtual InsertImage() = 0;
	virtual ~Piece();
private:

};



class Pawn : public Piece
{
public:
	Pawn(bool whiteElseBlack);
	void virtual InsertImage();
};



class Rook : public Piece
{
public:
	Rook::Rook(bool whiteElseBlack);
	void virtual InsertImage();
};


class Horse : public Piece
{
public:
	Horse::Horse(bool whiteElseBlack);
	void virtual InsertImage();
};



class Bishop : public Piece
{
public:
	Bishop::Bishop(bool whiteElseBlack);
	void virtual InsertImage();
};



class King : public Piece
{
public:
	King::King(bool whiteElseBlack);
	void virtual InsertImage();
};


class Queen : public Piece
{
public:
	Queen::Queen(bool whiteElseBlack);
	void virtual InsertImage();
};




class ChessPlayer
{
public:
	bool kingInThreat;
	bool whiteElseBlack;
	milliseconds timer;
	Piece* pieces[16];

	ChessPlayer(int time, bool whiteElseBlack);
	~ChessPlayer();

	bool GetKingInsThreat() const;

private:

};

#endif CHESS_PLAYER_CLASS_H