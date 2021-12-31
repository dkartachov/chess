#pragma once
#include "AssetManager.h"

class AudioManager {
public:
	static AudioManager* Instance();
	static void Release();

	AudioManager();
	~AudioManager();

	void PlaySFX(std::string filename, int loops = 0, int channel = -1);

private:
	static AudioManager* instance;
	AssetManager* assetManager;
};