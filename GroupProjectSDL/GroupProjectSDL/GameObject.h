#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Game.h"


//This is the class for each game object

class GameObject {

public:
	GameObject(const char* textureSheet);
	GameObject(const char* textureSheet, int xPos, int yPos, int height, int width);

	//Deconstructor
	~GameObject();

	void Update();
	void Render();

private:

	int xpos;
	int ypos;
	int srcRectW;
	int srcRectH;

	//To draw game object
	SDL_Texture* objectTexture;

	//To display game object
	SDL_Rect srcRect;
	SDL_Rect destRect;
};

#endif