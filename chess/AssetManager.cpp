#include "AssetManager.h"

AssetManager* AssetManager::instance = NULL;

AssetManager* AssetManager::Instance() {

	if (instance == NULL)
		instance = new AssetManager();
	return instance;
}

void AssetManager::Release() {
	delete instance;
	instance = NULL;
}

SDL_Texture* AssetManager::GetTexture(std::string filename) {
	std::string path = "Assets/";
	path.append(filename);

	if (textures[path] == nullptr) {
		textures[path] = Graphics::Instance()->LoadTexture(path);
	}

	return textures[path];
}

Mix_Chunk* AssetManager::GetSFX(std::string filename) {
	std::string path = "Assets/";
	path.append(filename);

	if (sfxs[path] == nullptr) {
		sfxs[path] = Mix_LoadWAV(path.c_str());

		if (sfxs[path] == nullptr) {
			printf("Error %s SFX %s could not be loaded", Mix_GetError(), filename.c_str());
		}
	}

	return sfxs[path];
}