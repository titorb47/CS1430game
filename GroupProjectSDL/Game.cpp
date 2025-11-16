#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

const char* PLAYER_PATH = "Assets/FishSheet.png";
const char* ENEMY_PATH = "Assets/Shark.png";


//1 Game.h declares addTile function
//2 Game.cpp defines addTile function
//3 addTile adds Tile component

//4 LoadMap is called in Game.cpp
//5 LoadMap calls addTile function
//6 addTile function calls addComponent function for
//	a tile component

const string MAP_PATH = "Assets/Tilemap.txt";
//To change MAP_ROWS or MAP_COLS, the file itself needs to be changed
//	to render any new tiles
const int MAP_ROWS = 32;
const int MAP_COLS = 32;
const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;

int Game::tileCount = 0;

//DO NOT CHANGE HEIGHT AND WIDTH
const int PLAYER_HEIGHT = 101;
const int PLAYER_WIDTH = 100;


//You can change scale
const int PLAYER_SCALE = 2;

const int ENEMY_HEIGHT = 144;
const int ENEMY_WIDTH = 144;

const int PLAYER_COLLIDER_HEIGHT = 75;
const int PLAYER_COLLIDER_WIDTH = 60;


const double PLAYER_SPEED = 2.5;


//Each new game object must be intialized, updated, and rendered
Map* map;
Manager manager;

//We add a new entity called player
//addEntity() also puts this entity in unique_ptr<Entity>
//so auto keyword is necessary to reference it again
auto& player(manager.addEntity());
auto& enemy(manager.addEntity());

//See ECS.cpp to understand how the group bitset works

//The type of this enum is set to size_t
enum groupLabels : size_t {
	MAP_GROUP,
	PLAYERS_GROUP,
	ENEMIES_GROUP,
	COLLIDERS_GROUP
};

/*
* The renderer is set to a null pointer before
* SDL is initialized
*/

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
vector<ColliderComponent*> Game::colliders;

//Constructor
Game::Game(){
	isRunning = false;
	window = NULL;
}

//Deconstructor;
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, bool fullscreen) {
	
	//default value of SDL_WINDOW
	int flags = 0;
	
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	//If initialization successful
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "ALL SYSTEMS GO!" << endl;

		//See game.h
		window = SDL_CreateWindow(title, xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

		//Can use window like a bool
		if (window) {
			cout << "Window created!" << endl;
		}

		//SDL_CreateRenderer(window, index, flags) 
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer created!" << endl;
		}

		isRunning = true;


		//map.h and map.cpp
		map = new Map();

		//See Map.cpp
		//Last two numbers are sizes for array
		Map::LoadMap(MAP_PATH, MAP_ROWS, MAP_COLS);

		//See ecs.h and the header files for each component
		player.addComponent<TransformComponent>(0, 0, PLAYER_SPEED, PLAYER_HEIGHT, PLAYER_WIDTH, 2);
		player.addComponent<SpriteComponent>(PLAYER_PATH, 4, 50);
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("player", PLAYER_COLLIDER_HEIGHT, PLAYER_COLLIDER_WIDTH);

		//Add player to PLAYER_GROUP
		player.addGroup(PLAYERS_GROUP);

		//x, y, speed, height, width, scale
		enemy.addComponent<TransformComponent>(300, 300, 0, ENEMY_HEIGHT, ENEMY_WIDTH, 2);
		enemy.addComponent<SpriteComponent>(ENEMY_PATH);
		enemy.addComponent<ColliderComponent>("enemy", 70, 60, 50, 20);
		
		//Add enemy to ENEMY_GROUP
		enemy.addGroup(ENEMIES_GROUP);
	}

	//Initialization was not successful
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {

	//Check and retrieve pending events
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update() {
	//This was just used to keep track of frames

	/*
	++count;
	cout << count << endl;
	*/


	//See ecs.h (manager and entity class)

	manager.refresh();
	manager.update();

	Vector2D currPosition = player.getComponent<TransformComponent>().position;

	int playerTop = player.getComponent<ColliderComponent>().topEdge;

	int playerLeft = player.getComponent<ColliderComponent>().leftEdge;

	int playerRight = player.getComponent<ColliderComponent>().rightEdge;

	int playerBottom = player.getComponent<ColliderComponent>().bottomEdge;

	for (auto cc : colliders) {
		if (Collision::AABB(player.getComponent<ColliderComponent>(),
			*cc) && cc->tag == "enemy")
		{
			int ccLeft = cc->leftEdge;
			int ccRight = cc->rightEdge;
			int ccTop = cc->topEdge;
			int ccBottom = cc->bottomEdge;


			int overlapRight = playerRight - ccLeft;   // overlap if player is to the left of wall
			int overlapLeft = ccRight - playerLeft;   // overlap if player is to the right of wall
			int overlapTop = ccBottom - playerTop;   // overlap if player is below wall
			int overlapBottom = playerBottom - ccTop;   // overlap if player is above wall

			// Only resolve if there is an actual collision
			if (overlapRight > 0 && overlapLeft > 0 && overlapTop > 0 && overlapBottom > 0) {

				// Find the smallest overlap axis
				int minOverlap = std::min({ overlapRight, overlapLeft, overlapTop, overlapBottom });

				//This resolve vector determines how much the player should move
				Vector2D resolve(0, 0);

				if (minOverlap == overlapRight)
					resolve = Vector2D(-overlapRight, 0);
				else if (minOverlap == overlapLeft)
					resolve = Vector2D(overlapLeft, 0);
				else if (minOverlap == overlapTop)
					resolve = Vector2D(0, -overlapTop);
				else if (minOverlap == overlapBottom)
					resolve = Vector2D(0, overlapBottom);

				currPosition += resolve;
				player.getComponent<TransformComponent>().velocity = Vector2D(0, 0);
			}
		}
	}
	

	
	if (playerLeft < 0) {
		Vector2D newVec(0 - playerLeft, 0);
		currPosition += newVec;
	}
	if ((playerRight) > WINDOW_WIDTH) {
		Vector2D newVec(playerRight - WINDOW_WIDTH, 0);
		currPosition -= newVec;
	}

	if (playerTop < 0) {
		Vector2D newVec(0, 0 - playerTop);
		currPosition += newVec;
	}

	if ((playerBottom) > WINDOW_HEIGHT){
		Vector2D newVec(0, playerBottom - WINDOW_HEIGHT);
		currPosition -= newVec;
	}

	player.getComponent<TransformComponent>().position = currPosition;
}

//Here we add our groups

/* tiles, players, and enemies are references
   to the vector of entity pointers */
auto& tiles(manager.GetGroup(MAP_GROUP));
auto& players(manager.GetGroup(PLAYERS_GROUP));
auto& enemies(manager.GetGroup(ENEMIES_GROUP));

void Game::render() {
	//Clear our game's renderer data member
	SDL_RenderClear(renderer);

	for (auto& t : tiles) {
		t->draw();
	}

	for (auto& p : players) {
		p->draw();
	}

	for (auto& e : enemies) {
		e->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	//This is how we clean up our game!
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game was cleaned!" << endl;
}

void Game::AddTile(int id, int x, int y) {
	//Adds the tile entity
	auto& tile(manager.addEntity());

	
	//Adds the tile component to the tile entity
	tile.addComponent<TileComponent>(x, y, TILE_WIDTH, TILE_HEIGHT, id);
	tile.addGroup(MAP_GROUP);
}

