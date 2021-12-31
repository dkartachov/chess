#pragma once
#include "Graphics.h"
#include <SDL_mixer.h>
#include <map>

class AssetManager {
public:
	static AssetManager* Instance();
	static void Release();

	SDL_Texture* GetTexture(std::string filename);
	Mix_Chunk* GetSFX(std::string filename);

private:
	static AssetManager* instance;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, Mix_Chunk*> sfxs;
};