#ifndef collision_h
#define collision_h

#include "SDL.h"

class Collision {
public:
	/*Axis Aligned Bounding Box*/
	static bool AABB(const SDL_Rect& rect1, const SDL_Rect& rect2); 

};

#endif