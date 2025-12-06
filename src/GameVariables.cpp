#include "GameVariables.h"

//THESE ARE THE DEFINED VARIABLES FOR THE ENTIRE GAME
//DECLARED IN GameConstants.h

SDL_Renderer* RENDERER = nullptr;

double GAME_START_DELAY = 3000;
bool GAME_STARTED = false;

const char* MAP_PATH = "Assets/Ocean Map v1.png";

const double DELTA_TIME = 1000/60;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;
const int DASH_DELAY = 3000;
const int DASH_DURATION = 1000;

const char* FONT_PATH = "Assets/font.ttf";
const int FONT_SIZE = 44;
const int FONT_R = 255;
const int FONT_G = 255;
const int FONT_B = 255;
const int FONT_A = 255;

const char* FONT_BACKGROUND_PATH = "Assets/FontBackground.png";
const double FONT_BACKGROUND_SCALE = 1.6;
const int SCORE_UPDATE_SLOWNESS = 100;

SDL_Texture* mapTexture;
const int MAP_DEST_RECT_X = 1024;
const int MAP_DEST_RECT_Y = 1024;
const int MAP_SRC_RECT_Y = 492;
const int MAP_SRC_RECT_W = 512;
const int MAP_SRC_RECT_H = 20;

const int MAP_SIZE_Y = 512;
const int MAP_SCROLL_SPEED = 10;

const int TEXT_H = 75;
const int TEXT_W = 75;
const int TEXT_START_X = 75;
const int TEXT_START_Y = 50;

const int SCORE_BACKGROUND_X = 0;
const int SCORE_BACKGROUND_Y = -40;
const int SCORE_BACKGROUND_H = 300;
const int SCORE_BACKGROUND_W = 250;

SDL_Rect MAP_DEST_RECT = { 0, 0, MAP_DEST_RECT_X, MAP_DEST_RECT_Y };
SDL_Rect MAP_SRC_RECT = { 0, MAP_SRC_RECT_Y, MAP_SRC_RECT_W, MAP_SRC_RECT_H };
const SDL_Rect TEXT_DEST_RECT = {TEXT_START_X, TEXT_START_Y, TEXT_W, TEXT_H};


const int TOP = 700;

//To change MAP_ROWS or MAP_COLS, the file itself needs to be changed
//	to render any new tiles
const int MAP_ROWS = 32;
const int MAP_COLS = 32;
const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;

vector<const char*> PLAYER_PATHS = {"Assets/EelSwim.png",
"Assets/EelAttack.png", "Assets/EelDeath.png"};
vector<const char*> ENEMY_PATHS = {"Assets/JellyfishSwim.png",
"Assets/JellyfishAttack.png"};

//DO NOT CHANGE HEIGHT AND WIDTH
const int PLAYER_HEIGHT = 48;
const int PLAYER_WIDTH = 48;

//You can change scale and speed
const int PLAYER_SCALE = 4;
const double PLAYER_SPEED = 2.6;
const double PLAYER_DASH_MULTIPLIER = 1.5;

const int ENEMY_SCALE = 8;
int ENEMY_SPAWNRATE_MODIFIER = 0;
int ENEMY_SPAWNRATE_MS = 2000;
const int ENEMY_HEIGHT = 48;
const int ENEMY_WIDTH = 48;
const int ENEMY_SPEED = 15;
const int ENEMY_SPAWNX_RANGE = 850;
const int ENEMY_COLLIDER_WIDTH = 50;
const int ENEMY_COLLIDER_HEIGHT = 80;

Vector2D enemyVector(0, 1);

const int PLAYER_COLLIDER_HEIGHT = 75;
const int PLAYER_COLLIDER_WIDTH = 60;
const double COLLIDER_SCALER = 0.25;

const string PLAYER_TAG = "player";
const string ENEMY_TAG = "enemy";

const int PLAYER_SWIM_ANIM_SPEED = 100;
const int PLAYER_DASH_ANIM_SPEED = 50;
const int PLAYER_DEATH_ANIM_SPEED = 400;
const int PLAYER_DEATH_ANIM_DURATION = 6 * PLAYER_DEATH_ANIM_SPEED;
const int ENEMY_SWIM_ANIM_SPEED = 100;

const string PLAYER_SWIM = "Swim";
const string PLAYER_DASH = "Dash";
const string PLAYER_DEATH = "Death";
const string ENEMY_SWIM = "Swim";

Animation playerSwim(0, 6, PLAYER_SWIM_ANIM_SPEED);
Animation playerDash(0, 6, PLAYER_DASH_ANIM_SPEED);
Animation playerDeath(0, 6, PLAYER_DEATH_ANIM_SPEED);
Animation enemySwim(0, 4, ENEMY_SWIM_ANIM_SPEED);

const map<string, Animation> PLAYER_ANIMATIONS = {
	{PLAYER_SWIM, playerSwim},
	{PLAYER_DASH, playerDash},
	{PLAYER_DEATH, playerDeath}
};
const map<string, Animation> ENEMY_ANIMATIONS = {
	{ENEMY_SWIM, enemySwim}
};