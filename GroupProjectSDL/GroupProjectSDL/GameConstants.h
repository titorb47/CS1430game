#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <vector>
#include <map>
#include "SDL.h"
#include "Vector2D.h"
#include "Animation.h"

///How adding tiles works
//1 Game.h declares addTile function
//2 Game.cpp defines addTile function
//3 addTile adds Tile component

//4 LoadMap is called in Game.cpp
//5 LoadMap calls addTile function
//6 addTile function calls addComponent function for
//	a tile component
extern const char* MAP_PATH;

extern SDL_Texture* mapTexture;
extern const int MAP_DEST_RECT_X;
extern const int MAP_DEST_RECT_Y;
extern const int MAP_SRC_RECT_Y; 
extern const int MAP_SRC_RECT_W;
extern const int MAP_SRC_RECT_H;

extern const int MAP_SIZE_Y;
extern const int MAP_SCROLL_SPEED;


extern SDL_Rect mapDestRect;
extern SDL_Rect mapSrcRect;

//To change MAP_ROWS or MAP_COLS, the file itself needs to be changed
//	to render any new tiles
extern const int MAP_ROWS;
extern const int MAP_COLS;
extern const int TILE_HEIGHT;
extern const int TILE_WIDTH;

extern vector<const char*> PLAYER_PATHS;
extern vector<const char*> ENEMY_PATHS;

//DO NOT CHANGE HEIGHT AND WIDTH
extern const int PLAYER_HEIGHT;
extern const int PLAYER_WIDTH;

//You can change scale and speed
extern const int PLAYER_SCALE;
extern const double PLAYER_SPEED;
extern const double PLAYER_DASH_MULTIPLIER;

extern const int ENEMY_SCALE;
extern const int ENEMY_SPAWNRATE_MS;
extern const int ENEMY_HEIGHT;
extern const int ENEMY_WIDTH;
extern const int ENEMY_SPEED;
extern const int ENEMY_SPAWNX_RANGE;
extern const int ENEMY_SPAWNY_RANGE;

extern Vector2D enemyVector;

extern const int PLAYER_COLLIDER_HEIGHT;
extern const int PLAYER_COLLIDER_WIDTH;
extern const double COLLIDER_SCALER;

extern const string PLAYER_TAG;
extern const string ENEMY_TAG;

extern const int PLAYER_SWIM_ANIM_SPEED;
extern const int PLAYER_DASH_ANIM_SPEED;
extern const int PLAYER_DEATH_ANIM_SPEED;
extern const int PLAYER_DEATH_ANIM_DURATION;
extern const int ENEMY_SWIM_ANIM_SPEED;

extern const string PLAYER_SWIM;
extern const string PLAYER_DASH;
extern const string PLAYER_DEATH;
extern const string ENEMY_SWIM;

extern Animation playerSwim;
extern Animation playerDash;
extern Animation playerDeath;
extern Animation enemySwim;

extern const map<string, Animation> PLAYER_ANIMATIONS;
extern const map<string, Animation> ENEMY_ANIMATIONS;


#endif