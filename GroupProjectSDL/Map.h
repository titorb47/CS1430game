#ifndef MAP_H
#define MAP_H

#include <string>
using namespace std;

class Map {
	public:

		Map();

		//Deconstructor
		~Map();

		//This needs to be static because Game::AddTile is also static

		static void LoadMap(string path, int sizeX, int sizeY);


	private:


};

#endif
