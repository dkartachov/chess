#include "AudioManager.h"

AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::Instance() {

	if (instance == nullptr) {
		instance = new AudioManager();
	}

	return instance;
}

void AudioManager::Release() {
	delete instance;
	instance = nullptr;
}

AudioManager::AudioManager() {
	assetManager = AssetManager::Instance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("Mixer initialization error: %s", Mix_GetError());
	}
}

AudioManager::~AudioManager() {
	assetManager = nullptr;
	Mix_Quit();
}

void AudioManager::PlaySFX(std::string filename, int loops, int channel) {
	Mix_PlayChannel(channel, assetManager->GetSFX(filename), loops);
}