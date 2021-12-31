#pragma once
#include "AudioManager.h"
#include "Piece.h"
#include <map>

class Game {
public:
	Game();

	void Run();

	void DrawBoard();
	void PrintBoard();

	void EarlyUpdate();
	void Update();
	void Render();
	void Quit();

private:
	bool Check();
	bool CheckMate();

private:
	Graphics* graphics;
	AudioManager* audioManager;

	std::map<Vector, Piece*> grid;
	SDL_Event event;

	// mouse
	bool holdingLMB;
	SDL_Point mousePos;
	Piece* selected;

	Piece::COLOR white;
	Piece::COLOR black;
	Piece::COLOR turn;

	bool check;
	bool quit;
};