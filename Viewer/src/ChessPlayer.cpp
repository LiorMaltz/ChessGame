#include "ChessPlayer.h"



// initialize player by set his given time, Color and pieces
ChessPlayer::ChessPlayer(const int& time, const bool& blackElseWhite) : timer(time), blackElseWhite(blackElseWhite), kingInThreat(false)
{
	// Create pieces
	int id = 0;
	pieces[0] = new King(blackElseWhite, id++);
	pieces[1] = new Queen(blackElseWhite, id++);
	pieces[2] = new Rook(blackElseWhite, id++);
	pieces[3] = new Rook(blackElseWhite, id++);
	pieces[4] = new Bishop(blackElseWhite, id++);
	pieces[5] = new Bishop(blackElseWhite, id++);
	pieces[6] = new Horse(blackElseWhite, id++);
	pieces[7] = new Horse(blackElseWhite, id++);
	for (int i = 8; i < NUM_OF_PIECES; i++)
	{
		pieces[i] = new Pawn(blackElseWhite, id++);
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
bool ChessPlayer::GetKingInThreat() const
{
	return kingInThreat;
}


// piece methods

Piece::Piece(const bool& blackElseWhite, const int& id) : blackElseWhite(blackElseWhite), pieceImage(NULL), pieceType(PieceType::DefaultType), id(id), model(NULL)
{
	locationOnBoard[0] = 0;
	locationOnBoard[1] = 1;
}

Piece::~Piece()
{}

void Piece::SetModel(MeshModel* model)
{
	this->model = model;
}

PieceType Piece::GetPieceType() const
{
	return pieceType;
}

int Piece::GetID() const
{
	return id;
}

void Piece::SetLocation(const int* loc)
{
	locationOnBoard[0] = loc[0];
	locationOnBoard[1] = loc[1];
}



Pawn::Pawn(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id) , firstMoove(true)
{
	pieceType = PieceType::PawnType;
	InsertImage();
}


void Pawn::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_Pawn.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_Pawn.png";
	}

}

void Pawn::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	//model->ScaleObject(vec3(0.125, 0.125, 0.125), true);
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;

	float steps = float(id) - 11.5f;
	if (steps > 0)
	{
		steps = floor(steps);
		model->TranslateObject(vec3(stepDist * steps + (stepDist / 2), (stepDist * 2.0f + (stepDist / 2)) * yPar + 0.001f, - 0.1), true);
	}
	else
	{
		steps = ceil(steps);
		model->TranslateObject(vec3(stepDist * steps - (stepDist / 2), (stepDist * 2.0f + (stepDist / 2)) * yPar + 0.001f, - 0.1), true);
	}

}

Rook::Rook(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id)
{
	pieceType = PieceType::RookType;
	InsertImage();
}

void Rook::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_Rook.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_Rook.png";
	}

}
void Rook::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;
	if (id == 2)
	{
		model->TranslateObject(vec3(stepDist * -3 - (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
	else
	{
		model->TranslateObject(vec3(stepDist * 3 + (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
}

Horse::Horse(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id)
{
	pieceType = PieceType::HorseType;
	InsertImage();
}

void Horse::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_Horse.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_Horse.png";
	}

}

void Horse::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;
	if (id == 6)
	{
		model->TranslateObject(vec3(stepDist * -2 - (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
	else
	{
		model->TranslateObject(vec3(stepDist * 2 + (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
}

Bishop::Bishop(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id)
{
	pieceType = PieceType::BishopType;
	InsertImage();
}

void Bishop::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_Bishop.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_Bishop.png";
	}

}

void Bishop::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;
	if (id == 4)
	{
		model->TranslateObject(vec3(stepDist * - 1 - (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
	else
	{
		model->TranslateObject(vec3(stepDist * 1 + (stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
	}
}

King::King(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id)
{
	pieceType = PieceType::KingType;
	InsertImage();
}

void King::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_King.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_King.png";
	}

}

void King::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;

	model->TranslateObject(vec3((stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
}

Queen::Queen(const bool& blackElseWhite, const int& id) : Piece(blackElseWhite, id)
{
	pieceType = PieceType::QueenType;
	InsertImage();
}

void Queen::InsertImage()
{
	int width, height, Channels;
	if (!blackElseWhite)
	{
		pieceImage = "../Data/ChessImages/White_Queen.png";
	}
	else
	{
		pieceImage = "../Data/ChessImages/Black_Queen.png";
	}

}

void Queen::InitializeModelPlace(const float& stepDist, const float& scale)
{
	// First scale the model
	model->ScaleObject(vec3(scale, scale, scale), true);

	// Translate to the right place
	int yPar;
	if (!blackElseWhite)
		yPar = -1;
	else
		yPar = 1;

	model->TranslateObject(vec3((-stepDist / 2), (stepDist * 3.0f + (stepDist / 2)) * yPar + 0.001f, -0.1), true);
}