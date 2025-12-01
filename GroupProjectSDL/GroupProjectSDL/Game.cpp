#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <cstdlib>
#include "GameConstants.h"


bool Game::playerIsAlive = true;
bool dying = false;
Uint32 Game::currentTime = 0;
int lastSpawn = 0;


//Each new game object must be intialized, updated, and rendered
//Map* gameMap;
Manager manager;

//We add a new entity called player and map
//addEntity() also puts this entity in unique_ptr<Entity>
//so auto keyword is necessary to reference it again
auto& player(manager.addEntity());


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
		//gameMap = new Map();

		//See Map.cpp
		//Last two numbers are sizes for array
		//Map::LoadMap(MAP_PATH, MAP_ROWS, MAP_COLS);

		mapTexture = TextureManager::LoadTexture(MAP_PATH);

		//See ecs.h and the header files for each component
		//ALWAYS DEFINE TRANSFORM FIRST, AND COLLIDER BEFORE SPRITE
		player.addComponent<TransformComponent>(500, 850, PLAYER_SPEED, PLAYER_HEIGHT, 
		PLAYER_WIDTH, PLAYER_SCALE);
		player.getComponent<TransformComponent>().setTag(PLAYER_TAG);
		player.addComponent<SpriteComponent>(PLAYER_PATHS, -90, PLAYER_ANIMATIONS);
		player.addComponent<ColliderComponent>(PLAYER_TAG, PLAYER_COLLIDER_HEIGHT, PLAYER_COLLIDER_WIDTH);
		player.addComponent<KeyboardController>();


		//Add player to PLAYER_GROUP
		player.addGroup(PLAYERS_GROUP);



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

	//This is the timer that spawns our enemies
	// and carries out animations
	currentTime = SDL_GetTicks();

	//See ecs.h (manager and entity class)
	manager.refresh();
	manager.update();

	//Get the parts of the player's collider component
	Vector2D currPosition = player.getComponent<TransformComponent>().position;
	int playerTop = player.getComponent<ColliderComponent>().topEdge;
	int playerLeft = player.getComponent<ColliderComponent>().leftEdge;
	int playerRight = player.getComponent<ColliderComponent>().rightEdge;
	int playerBottom = player.getComponent<ColliderComponent>().bottomEdge;

	for (auto cc : colliders) {
		if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)
		&& cc->tag != PLAYER_TAG)
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
				Vector2D resolve;
				resolve.Zero();

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

			//If the player hits an enemy, stop animations by setting 
			//playerIsAlive to false

			if (cc->tag == "enemy") {
				playerIsAlive = false;
				player.getComponent<SpriteComponent>().Play(PLAYER_DEATH, 2);
			}
		}
	}
	
	//For collisions against the wall
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


	if ( (currentTime - lastSpawn >= ENEMY_SPAWNRATE_MS) && 
	playerIsAlive) {

		auto& enemy(manager.addEntity());

		//x, y, speed, height, width, scale
		enemy.addComponent<TransformComponent>(rand() % ENEMY_SPAWNX_RANGE, 
		-100, ENEMY_SPEED, ENEMY_HEIGHT, 
		ENEMY_WIDTH, ENEMY_SCALE);

		TransformComponent& transform = enemy.getComponent<TransformComponent>();

		enemy.addComponent<SpriteComponent>(ENEMY_PATHS, 180, ENEMY_ANIMATIONS);
		enemy.addComponent<ColliderComponent>(ENEMY_TAG, 100, 100);

		enemy.getComponent<SpriteComponent>().Play(ENEMY_SWIM, 0);

		//Add enemy to ENEMY_GROUP
		enemy.addGroup(ENEMIES_GROUP);

		transform.velocity += enemyVector;

		lastSpawn = currentTime;
	}

	if (playerIsAlive) {
		//Scroll the Map
		mapSrcRect.y = MAP_SIZE_Y - 20 - 
		( (currentTime / MAP_SCROLL_SPEED) % (MAP_SIZE_Y));

		//Play swimming animation
		if (!player.getComponent<KeyboardController>().dashing) {
			player.getComponent<SpriteComponent>().Play(PLAYER_SWIM, 0);
		}
	}


}

//Here we add our groups

/* players and enemies are references
   to the vector of entity pointers */
auto& players(manager.GetEntityInGroup(PLAYERS_GROUP));
auto& enemies(manager.GetEntityInGroup(ENEMIES_GROUP));

void Game::render() {


	//Clear our game's renderer data member
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, mapTexture, &mapSrcRect, &mapDestRect);

	for (auto& p : players) {
		p->draw();
	}

	for (auto& e : enemies) {
		if (e->getComponent<TransformComponent>().position.y < 1100) {
			e->draw();
		}
		else {
			e->destroy();

		}
		
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
