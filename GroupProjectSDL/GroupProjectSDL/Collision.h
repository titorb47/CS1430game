#ifndef collision_h
#define collision_h

#include "SDL.h"
#include "ColliderComponent.h"

class Collision {
public:
	/*Axis Aligned Bounding Box*/
	static bool AABB(const SDL_Rect& rect1, const SDL_Rect& rect2); 
	
	static bool AABB(const ColliderComponent& colliderA, 
	const ColliderComponent& colliderB);

	/*Comparing colliding objects*/

};

#endif