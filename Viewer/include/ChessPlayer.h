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
	

	MeshModel* model;
	Piece(const bool& blackElseWhite, const int& id);
	PieceType GetPieceType() const;
	int GetID() const;
	void SetModel(MeshModel* model);
	void SetLocation(const int* loc);
	virtual void InsertImage() = 0;
	virtual void InitializeModelPlace(const float& stepDist, const float& scale) = 0;
	virtual ~Piece();
private:

};



class Pawn : public Piece
{
public:
	Pawn(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	virtual void InitializeModelPlace(const float& stepDist, const float& scale);
	bool firstMoove;
};



class Rook : public Piece
{
public:
	Rook::Rook(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace(const float& stepDist, const float& scale);
};


class Horse : public Piece
{
public:
	Horse::Horse(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace(const float& stepDist, const float& scale);
};



class Bishop : public Piece
{
public:
	Bishop::Bishop(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace(const float& stepDist, const float& scale);
};



class King : public Piece
{
public:
	King::King(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace(const float& stepDist, const float& scale);
};


class Queen : public Piece
{
public:
	Queen::Queen(const bool& blackElseWhite, const int& id);
	void virtual InsertImage();
	void InitializeModelPlace(const float& stepDist, const float& scale);
};




class ChessPlayer
{
public:
	bool kingInThreat;
	bool blackElseWhite;
	milliseconds timer;
	Piece* pieces[16];

	ChessPlayer(const int& time, const bool& blackElseWhite);
	~ChessPlayer();

	bool GetKingInThreat() const;

private:

};

#endif CHESS_PLAYER_CLASS_H