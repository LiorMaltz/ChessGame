#ifndef CHESS_MATCH_CLASS_H
#define CHESS_MATCH_CLASS_H

#include"ChessPlayer.h"
#include <stack>
#include <array>



#define BOARD_ROWS 8
#define BOARD_COLS 8
enum Turn {WHITE, BLACK};

struct ChessBoardCell
{
	bool cellIsOccupied;
	// flase means white piece on the cell, true is for black
	bool blackElseWhiteIsOn;
	Piece* piece;

	ChessBoardCell() : blackElseWhiteIsOn(false), cellIsOccupied(false), piece(NULL) {}
};


// type that will be usable for coloring the table
struct TableColorType {
	int data[8][8] = { 0 };
	std::array<int, 2> currentSquere = { -1, -1 };
	std::array<int, 2> ClickedSquere = { -1, -1 };
	std::stack<std::array<int, 2>> MoveToSqueresFromClicked;
	bool change = false;
	bool clicked = false;
};


class ChessMatch
{
public:
	char* chessBoardImage;
	// Defualt/costumize constructor
	ChessMatch(const int& time = 10);
	~ChessMatch();

	// If the move was executed return true, else return false
	bool Move(const int* from, const int* to);
	// Function to move the pieces on the board. will recieve two int[2] arrays that represet 'from' coordintes and 'to' coordintes on the chess table
	void MakeMove(const int* from, const int* to);
	// true - the move is legal, false the move is not legal
	bool TestMove(const int* from, const int* to);
	// returns: 1 - there is a mate, 2 - there is a draw, false - game can continue
	int MateOrDraw();
	// true - check on king. false - no check
	bool CheckKingThreat();
	void ConstructChessBoardBuffer();

	ChessPlayer* GetPlayer(const int& num);
	TableColorType* GetTableColorType();

	// Graphic variables
	TableColorType chessBoardColorData;
	GLuint tableColorBuffer;
	GLuint tableColorBindingPlace;
	float stepDistOnForSquere = 0.125028f;
private:
	// False is white turn, true is black
	Turn turn;		
	ChessPlayer* players[2];
	// With the data which is stored in the cell we could decide if a player move is legal and execute it
	ChessBoardCell chessBoard[BOARD_ROWS][BOARD_COLS];



};



#endif // !CHESS_MATCH_CLASS_H

