#ifndef CHESS_PLAYER_CLASS_H
#define CHESS_PLAYER_CLASS_H
#include<chrono>
#include "stb_image.h"
#include "MeshModel.h"

using namespace std::chrono;

#define NUM_OF_PIECES 16

enum class PieceType { KingType, QueenType, RookType, HorseType, BishopType, PawnType, DefaultType };


// Making abstact class for all the chess pieces
class Piece
{
public:
	char* pieceImage;
	PieceType pieceType;
	bool blackElseWhite;
	int id;
	int locationOnBoard[2];
	float stepDist;
	float modelScale;
	

	MeshModel* model;
	Piece(const bool& blackElseWhite, const int& id);
	PieceType GetPieceType() const;
	int GetID() const;
	void SetModel(MeshModel* model);
	void SetLocation(const int* loc);
	void MoveModelToLocation(const int* to);
	virtual void InsertImage() = 0;
	virtual void InitializeModelPlace() = 0;
	virtual ~Piece();
private:

};



class Pawn : public Piece
{
public:
	Pawn(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	virtual void InitializeModelPlace();
	bool firstMoove;
};



class Rook : public Piece
{
public:
	Rook::Rook(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace();
	bool firstMoove;
};


class Horse : public Piece
{
public:
	Horse::Horse(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace();
};



class Bishop : public Piece
{
public:
	Bishop::Bishop(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace();
};



class King : public Piece
{
public:
	King::King(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace();
	bool firstMoove;
};


class Queen : public Piece
{
public:
	Queen::Queen(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace();
};




class ChessPlayer
{
public:
	bool kingInThreat;
	bool blackElseWhite;
	milliseconds timer;
	Piece* pieces[16];

	// En passant variables
	int enPassantlocation[2];
	bool enPassantBool;

	ChessPlayer(const int& time, const bool& blackElseWhite);
	~ChessPlayer();

	bool GetKingInThreat() const;

private:

};

#endif CHESS_PLAYER_CLASS_H