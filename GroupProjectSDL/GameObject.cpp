#include "GameObject.h"
#include "TextureManager.h"

/*
* We use pass by pointer because these can be null
* and reassigned unlike pass by reference
*/

GameObject::GameObject(const char* textureSheet) {
	objectTexture = TextureManager::LoadTexture(textureSheet);

	xpos = 0;
	ypos = 0;

	//Rects are structs with {x,y,width,height}

	srcRect = {0, 0, 0 ,0};
	destRect = {0, 0 ,0, 0};

	srcRectH = 32;
	srcRectW = 32;
}

GameObject::GameObject(const char* textureSheet, int xpos, int ypos, int height, int width) {
	objectTexture = TextureManager::LoadTexture(textureSheet);

	this->xpos = xpos;
	this->ypos = ypos;

	srcRect = { 0, 0, 0 ,0 };
	destRect = { 0, 0 ,0, 0 };

	srcRectH = height;
	srcRectW = width;
}


GameObject::~GameObject() {}

void GameObject::Update() {
	//This is what is causing our game objects to move
	xpos++;
	ypos++;

	srcRect.h = srcRectH;
	srcRect.w = srcRectW;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;

}

void GameObject::Render() {

	//This adds the stuff to render
	//srcRect is portion of image to draw
	//destRect is where to draw
	
	/*
	* srcRect and destRect usually NULL
	* because srcRect = NULL means draw the 
	* entire image and destRect = NULL draws
	* the rect on the entire render frame
	*/

	//See Game.h and Game.cpp
	SDL_RenderCopy(Game::renderer, objectTexture, &srcRect, &destRect);
}