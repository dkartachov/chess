#include "Sprite.h"

Sprite::Sprite(const char* filename, Vector clipPos, Vector clipSize) {

	texture = AssetManager::Instance()->GetTexture(filename);

	clip.x = clipPos.x;
	clip.y = clipPos.y;
	clip.w = clipSize.x;
	clip.h = clipSize.y;
}

void Sprite::Render() {
	Graphics::Instance()->DrawTexture(texture, &clip, &rect);
}