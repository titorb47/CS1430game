#include "Map.h"
#include "TextureManager.h"

const char* LIGHT_WATER_PATH = "Assets/LightWater.png";
const char* MEDIUM_WATER_PATH = "Assets/MediumWater.png";
const char* DARK_WATER_PATH = "Assets/DarkWater.png";
const char* BUBBLES_PATH = "Assets/Bubbles.png";

enum TileType {
	LIGHT_WATER = 0,
	MEDIUM_WATER = 1,
	DARK_WATER = 2,
	BUBBLES = 3
};

//xMap and yMap are static const ints in Map.h
int mapLevel[Map::xMap][Map::yMap]{

	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },

};

Map::Map() {

	lightWater = TextureManager::LoadTexture(LIGHT_WATER_PATH);
	mediumWater = TextureManager::LoadTexture(MEDIUM_WATER_PATH);
	darkWater = TextureManager::LoadTexture(DARK_WATER_PATH);
	bubbles = TextureManager::LoadTexture(BUBBLES_PATH);

	LoadMap(mapLevel);

	/*
	* srcRect dimensions must equal
	* the dimensions of the object
	*/

	srcRect.x = srcRect.y = 0;
	srcRect.h = destRect.h = tileSize;
	srcRect.w = destRect.w = tileSize;

	destRect.x = destRect.y = 0;

}

Map::~Map(){
	SDL_DestroyTexture(lightWater);
	SDL_DestroyTexture(mediumWater);
	SDL_DestroyTexture(darkWater);
	SDL_DestroyTexture(bubbles);
};

void Map::LoadMap(int arr[xMap][yMap]) {
	

	for (int row = 0; row < xMap; ++row) {
		
		for (int col = 0; col < yMap; ++col){
		
		/*
		* Our map data member is assigned to the argument's map.
		* The argument is the map defined above, and LoadMap()
		* is called in the constructor
		*/
		

			map[row][col] = arr[row][col];
		}
	}
}

void Map::DrawMap() {

	TileType type;

	for (int row = 0; row < xMap; ++row) {

		for (int col = 0; col < yMap; ++col) {
		
			type = static_cast<TileType>(map[row][col]);

			destRect.x = col * Map::tileSize;
			destRect.y = row * Map::tileSize;

			switch (type) {
				case 0:
					TextureManager::Draw(lightWater, srcRect, destRect);
					break;
				case 1:
					TextureManager::Draw(mediumWater, srcRect, destRect);
					break;
				case 2:
					TextureManager::Draw(darkWater, srcRect, destRect);
					break;
				case 3: 
					TextureManager::Draw(bubbles, srcRect, destRect);
					break;
				default:
					break;
			}

		}
	}
}

