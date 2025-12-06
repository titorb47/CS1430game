#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	

	//To render images, you first need to load an image as a surface.
	//Then you can create a texture from the surface
	//See TextureManager.cpp and TextureManager.h


	SDL_Surface* surface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(RENDERER, surface);
	SDL_FreeSurface(surface);

	return texture;

}

SDL_Texture* TextureManager::LoadFont(TTF_Font& font, const char* text, int r, 
int g, int b, int a) {
	SDL_Color textColor = { r, g, b, a }; // White color (RGBA)
	SDL_Surface* textSurface = TTF_RenderText_Blended(&font, text, textColor);
	SDL_Texture* texture  = SDL_CreateTextureFromSurface(RENDERER, textSurface);
	SDL_FreeSurface(textSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, const SDL_Rect src, const SDL_Rect dest,
	double angle) {
	SDL_RenderCopyEx(RENDERER, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}
