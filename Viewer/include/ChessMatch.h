#ifndef CHESS_MATCH_CLASS_H
#define CHESS_MATCH_CLASS_H

#include"ChessPlayer.h"


#define BOARDROWS 8
#define BOARDCOLS 8

struct ChessBoardCell
{
	bool cellIsOccupied;
	// flase means white piece on the cell, true is for black
	bool blackElseWhiteIsOn;
	Piece* piece;

	ChessBoardCell() : blackElseWhiteIsOn(false), cellIsOccupied(false), piece(NULL) {}
};

class ChessMatch
{
public:
	unsigned char* chessBoardImage;
	// Defualt/costumize constructor
	ChessMatch(ChessPlayer* player0 = new ChessPlayer(10, 1), ChessPlayer* player1 = new ChessPlayer(10, 0), unsigned char* chessBoardImage = NULL);
	~ChessMatch();

	// Function to move the pieces. will recieve two int[2] arrays that represet from cood and to coord on the chess table
	void MakeMove(int* from, int* to);

private:
	bool turn;						// False is white turn, true is black
	ChessPlayer* players[2];
	// With the data which is stored in the cell we could decide if a player move is legal and execute it
	ChessBoardCell chessBoard[BOARDROWS][BOARDCOLS];

};



#endif // !CHESS_MATCH_CLASS_H

