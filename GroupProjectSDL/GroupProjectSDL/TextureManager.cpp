#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	

	//To render images, you first need to load an image as a surface.
	//Then you can create a texture from the surface
	//See TextureManager.cpp and TextureManager.h


	SDL_Surface* surface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);

	return texture;

}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest) {
	
	SDL_RenderCopy(Game::renderer, texture, &src, &dest);

}