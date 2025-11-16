#include "Map.h"
#include "Game.h"
#include <fstream>
#include <string>




using namespace std;
fstream mapFile;



Map::Map() {

	

}

Map::~Map(){

};

void Map::LoadMap(string path, int sizeX, int sizeY) {
	char tile;
	char comma;
	mapFile.open(path);

	if (!mapFile.is_open()) {
		cout << "ERROR: COULD NOT OPEN MAP! " << endl;
	}

	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			
			//get function in stream classes extracts a single character
			mapFile >> tile >> comma;

			//atoi converts from char to int
			Game::AddTile(atoi(&tile), x * 32, y * 32);
		}
	}

	mapFile.close();

}




