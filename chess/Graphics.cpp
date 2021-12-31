#include "Graphics.h"

Graphics* Graphics::instance = nullptr;

Graphics* Graphics::Instance() {
	if (instance == nullptr)
		instance = new Graphics();

	return instance;
}

void Graphics::Release() {
	delete instance;
	instance = nullptr;
}

Graphics::Graphics() {
	Init();
}

bool Graphics::Init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Cannot create window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Cannot create renderer: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	
	return true;
}

SDL_Texture* Graphics::LoadTexture(std::string path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		printf("ERROR Could not load image: %s\n", IMG_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		printf("ERROR Could not create texture from surface: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	return texture;
}

void Graphics::DrawTexture(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* rect, float angle, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, clip, rect, angle, NULL, flip);
}

void Graphics::DrawRect(SDL_Rect rect, int r, int g, int b, int alpha) {
	SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
	SDL_RenderFillRect(renderer, &rect);
}

void Graphics::Clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Graphics::Render() {
	SDL_RenderPresent(renderer);
}

void Graphics::Quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}