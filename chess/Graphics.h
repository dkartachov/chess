#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Vector.h"
#include <string>
#include <vector>

class Graphics {
public:
	static Graphics* Instance();
	static void Release();

	Graphics();
	bool Init();

	SDL_Texture* LoadTexture(std::string path);
	void DrawTexture(SDL_Texture* texture, SDL_Rect* clip = NULL, SDL_Rect* rect = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawRect(SDL_Rect rect, int r, int g, int b, int alpha = 255);
	void Clear();
	void Render();
	void Quit();
private:
	const char* TITLE = "Chess";
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 1024;

	SDL_Window* window;
	SDL_Renderer* renderer;
	static Graphics* instance;
};