#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Game.h"

//This class manages textures of game objects

class TextureManager {
	public:
		static SDL_Texture* LoadTexture(const char* filename);

		static SDL_Texture* LoadFont(TTF_Font& font, const char* text, int r, int g, int b, int a);

		static void Draw(SDL_Texture* texture, const SDL_Rect src, const SDL_Rect dest,
		const double angle);

};



#endif