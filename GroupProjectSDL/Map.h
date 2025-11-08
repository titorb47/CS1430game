#ifndef MAP_H
#define MAP_H

#include "Game.h"

struct Template {
	int srcRectH;
	int srcRectW;
};

class Map {
	public:

		Map();

		//Deconstructor
		~Map();

		static const int tileSize = 32;
		static const int xMap = Game::WINDOW_HEIGHT / tileSize;
		static const int yMap = Game::WINDOW_WIDTH / tileSize;

		void LoadMap(int[xMap][yMap]);
		void DrawMap();


	private:

		SDL_Rect srcRect;
		SDL_Rect destRect;


		SDL_Texture* lightWater;
		SDL_Texture* mediumWater;
		SDL_Texture* darkWater;
		SDL_Texture* bubbles;

		//Dimensions for Map?
		int map[xMap][yMap];
};

#endif
