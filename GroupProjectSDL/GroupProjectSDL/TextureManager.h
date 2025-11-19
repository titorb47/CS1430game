#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Game.h"

//This class manages textures of game objects

class TextureManager {
	public:
		static SDL_Texture* LoadTexture(const char* filename);

		static void Draw(SDL_Texture* texture, const SDL_Rect src, const SDL_Rect dest);

		static void DrawAngle(SDL_Texture* texture, const SDL_Rect src, const SDL_Rect dest,
		const double angle);

};



#endif