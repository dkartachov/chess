#include "Piece.h"

Piece::Piece(int x, int y, COLOR color, TYPE type) {
	firstMove = true;
	this->type = type;
	this->color = color;

	setPieceTypes();

	position = { x, y };

	sprite->rect.w = clipSize.x;
	sprite->rect.h = clipSize.y;

	sprite->rect.w = sprite->rect.w * 0.4;
	sprite->rect.h = sprite->rect.h * 0.4;
	sprite->rect.x = 64 + 128 * (position.x - 1) - sprite->rect.w / 2;
	sprite->rect.y = 64 + 128 * (position.y - 1) - sprite->rect.h / 2;
}

void Piece::setPieceTypes() {
	clipPos = { 0, 0 };
	clipSize = { 250, 300 };

	switch (type) {
	case TYPE::pawn:
		clipPos = { 1700, color == COLOR::white ? 0 : 300 };
		break;
	case TYPE::rook:
		clipPos = { 1350, color == COLOR::white ? 0 : 300 };
		break;
	case TYPE::knight:
		clipPos = { 1000, color == COLOR::white ? 0 : 300 };
		break;
	case TYPE::bishop:
		clipPos = { 675, color == COLOR::white ? 0 : 300 };
		break;
	case TYPE::queen:
		clipPos = { 310, color == COLOR::white ? 0 : 300 };
		clipSize.x = 300;
		break;
	case TYPE::king:
		clipPos = { 0, color == COLOR::white ? 0 : 300 };
		clipSize.x = 260;
		break;
	}

	sprite = new Sprite("sheet.png", clipPos, clipSize);
}

void Piece::Drag(SDL_Point point, bool checking) {
	dragged = checking ? Vector(point.x, point.y) : Vector(point.x / 128 + 1, point.y / 128 + 1 );

	if (!checking) {
		sprite->rect.x = point.x - sprite->rect.w / 2;
		sprite->rect.y = point.y - sprite->rect.h / 2;
	}
}

Vector Piece::GetDraggedPosition() {
	return dragged;
}

bool Piece::ValidMove(std::map<Vector, Piece*> grid) {

	switch (type) {
	case TYPE::pawn:
		return isValidPawnMove(grid);
		break;
	case TYPE::rook:
		return isValidRookMove(grid);
		break;
	case TYPE::knight:
		return isValidKnightMove(grid);
		break;
	case TYPE::bishop:
		return isValidBishopMove(grid);
		break;
	case TYPE::queen:
		return isValidQueenMove(grid);
		break;
	case TYPE::king:
		return isValidKingMove(grid);
		break;
	}

	return false;
}

bool Piece::isValidPawnMove(std::map<Vector, Piece*> grid) {
	Vector pos0 = { GetPosition().x, GetPosition().y };
	Vector pos = { GetDraggedPosition().x, GetDraggedPosition().y };
	Vector delta = pos - pos0;

	delta.y = color == COLOR::black ? delta.y : -delta.y;

	if (delta.y > 0 && pos.x == pos0.x && !grid[{pos.x, pos.y}]) {
		if (firstMove && delta.y == 2) {
			int ym = (pos0.y + pos.y) / 2;

			if (grid[{pos0.x, ym}]) {
				return false;
			}

			return true;
		}
		else if (delta.y <= 1) {
			return true;
		}
	}
	else if (delta.y == 1 && abs(delta.x) == 1) {
		if (grid[pos]) {
			if (grid[pos]->color != color) {
				return true;
			}
		}
	}

	return false;
}

bool Piece::isValidRookMove(std::map<Vector, Piece*> grid) {
	int dx, dy;

	dy = GetDraggedPosition().y - GetPosition().y;
	dx = GetDraggedPosition().x - GetPosition().x;

	if (abs(dy) > 0 && abs(dx) == 0 || abs(dx) > 0 && abs(dy) == 0) {
		int first, last;

		if (abs(dx) == 0) {
			first = dy > 0 ? GetPosition().y : GetDraggedPosition().y;
			last = dy > 0 ? GetDraggedPosition().y : GetPosition().y;
		}
		else {
			first = dx > 0 ? GetPosition().x : GetDraggedPosition().x;
			last = dx > 0 ? GetDraggedPosition().x : GetPosition().x;
		}
		
		for (int pos = first + 1; pos < last; pos++) {
			if (abs(dx) == 0 ? grid[{GetPosition().x, pos}] : grid[{pos, GetPosition().y}]) {
				return false;
			}
		}

		if (grid[{GetDraggedPosition().x, GetDraggedPosition().y}]) {
			if (grid[{GetDraggedPosition().x, GetDraggedPosition().y}]->color != color) {
				return true;
			}
			return false;
		}

		return true;
	}

	return false;
}

bool Piece::isValidKnightMove(std::map<Vector, Piece*> grid) {
	int x, y, dx, dy;

	x = GetDraggedPosition().x;
	y = GetDraggedPosition().y;
	dx = x - GetPosition().x;
	dy = y - GetPosition().y;

	if (abs(dx) > 2 || abs(dy) > 2) {
		return false;
	}

	if (grid[{x, y}] && grid[{x, y}]->color == color) {
		return false;
	}

	if (dx > 0) {
		if (dy > 0) {
			if (dx == 1 && dy == 2 || dx == 2 && dy == 1) {
				return true;
			}
			return false;
		}
		else {
			if (dx == 1 && dy == -2 || dx == 2 && dy == -1) {
				return true;
			}
			return false;
		}
	}
	else {
		if (dy > 0) {
			if (dx == -1 && dy == 2 || dx == -2 && dy == 1) {
				return true;
			}
			return false;
		}
		else {
			if (dx == -1 && dy == -2 || dx == -2 && dy == -1) {
				return true;
			}
			return false;
		}
	}

	return false;
}

bool Piece::isValidBishopMove(std::map<Vector, Piece*> grid) {
	Vector pos0 = { GetPosition().x, GetPosition().y };
	Vector pos = { GetDraggedPosition().x, GetDraggedPosition().y };
	Vector delta = pos - pos0;

	if (grid[pos] && grid[pos]->color == color) {
		return false;
	}

	if (abs(delta.x) == abs(delta.y) && delta != Vector::Zero()) {
		int dx, dy;

		if (delta.x > 0) {
			dx = 1;
			dy = delta.y > 0 ? 1 : -1;
		}
		else {
			dx = -1;
			dy = delta.y > 0 ? 1 : -1;
		}

		Vector square = pos0 + Vector(dx, dy);
		
		while (square != pos) {

			if (grid[square]) {
				return false;
			}

			square += Vector(dx, dy);
		}

		return true;
	}

	return false;
}

bool Piece::isValidQueenMove(std::map<Vector, Piece*> grid) {
	return isValidBishopMove(grid) || isValidRookMove(grid);
}

bool Piece::isValidKingMove(std::map<Vector, Piece*> grid) {
	Vector pos0 = { GetPosition().x, GetPosition().y };
	Vector pos = { GetDraggedPosition().x, GetDraggedPosition().y };
	Vector delta = pos - pos0;

	if (grid[pos] && grid[pos]->color == color) {
		return false;
	}

	if (delta != Vector::Zero() && abs(delta.x) < 2 && abs(delta.y) < 2) {
		return true;
	}

	return false;
}

void Piece::PositionOnBoard(Vector position) {
	firstMove = false;

	sprite->rect.x = (position.x - 0.5) * 128 - sprite->rect.w / 2;
	sprite->rect.y = (position.y - 0.5) * 128 - sprite->rect.h / 2;
	this->position = position;
}

Vector Piece::GetPosition() {
	return position;
}

void Piece::ReturnToPosition() {
	sprite->rect.x = (position.x - 0.5) * 128 - sprite->rect.w / 2;
	sprite->rect.y = (position.y - 0.5) * 128 - sprite->rect.h / 2;
}

int Piece::getW() {
	return sprite->rect.w;
}

int Piece::getH() {
	return sprite->rect.h;
}

SDL_Rect* Piece::getPiece() {
	return &sprite->rect;
}

void Piece::Update() {

}

void Piece::Render() {
	sprite->Render();
}