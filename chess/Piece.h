#pragma once
#include "Sprite.h"

class Piece {
public:
	enum class TYPE {pawn, rook, knight, bishop, queen, king};
	enum class COLOR {white, black};
	bool firstMove;
	TYPE type;
	COLOR color;

	Piece(int x, int y, COLOR color, TYPE type);

	void Drag(SDL_Point point, bool checking = false);
	Vector GetDraggedPosition();

	bool ValidMove(std::map<Vector, Piece*> grid);

	void PositionOnBoard(Vector position);
	Vector GetPosition();

	void ReturnToPosition();

	int getW();
	int getH();

	SDL_Rect* getPiece();

	void Update();
	void Render();

private:
	void setPieceTypes();
	bool isValidPawnMove(std::map<Vector, Piece*> grid);
	bool isValidRookMove(std::map<Vector, Piece*> grid);
	bool isValidKnightMove(std::map<Vector, Piece*> grid);
	bool isValidBishopMove(std::map<Vector, Piece*> grid);
	bool isValidQueenMove(std::map<Vector, Piece*> grid);
	bool isValidKingMove(std::map<Vector, Piece*> grid);

private:
	Vector clipPos;
	Vector clipSize;
	Vector position;
	Vector dragged;
	
	Sprite* sprite;
};