#pragma once
#include "AssetManager.h"

class Sprite {
public:
	Sprite(const char* filename, Vector clipPos, Vector clipSize);

	void Render();

public:
	SDL_Rect rect;
	SDL_Rect clip;
	SDL_Texture* texture;
};