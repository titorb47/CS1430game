#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class ColliderComponent;

//This is the class for the entire game

/*
* This is also the class from which all 
* other classes are derived 
*/

class Game {
public:
	Game();
	//Deconstructor for when game closes
	
	/*
	* Deconstructors deallocates memory to
	* prevent memory leaks
	*/

	~Game();

	void init();
	
	void runGame();

	void handleEvents();
	void update();
	void render();
	void clean();


	static bool running() { return isRunning; };
	static void setRunning(bool set){ isRunning = set; }


	static Uint32 currentTime;
	static bool playerIsAlive;

	static SDL_Renderer *renderer;

	static int score;


	/* Only one event is necessary, which we will keep checking
	   while the game is running */
	static SDL_Event event;

	static vector<ColliderComponent*> colliders;

private:
	static bool isRunning;
	SDL_Window* window = NULL;
	Uint32 frameStart = 0;
	int frameTime = 0;
};

#endif