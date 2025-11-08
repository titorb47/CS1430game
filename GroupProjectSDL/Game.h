#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

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

	void init(const char* title, int xpos, int ypos, bool fullscreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	static SDL_Renderer *renderer;

	static const int WINDOW_WIDTH = 800;
	static const int WINDOW_HEIGHT = 640;

	//Only one event is necessary
	static SDL_Event event;



private:
	bool isRunning;
	SDL_Window* window;
	int count;
};

#endif