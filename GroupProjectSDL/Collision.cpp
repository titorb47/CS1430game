#include "Collision.h"

bool Collision::AABB(const SDL_Rect& rec1, const SDL_Rect& rec2) {

	if (
		rec1.x + rec1.w >= rec2.x &&
		rec1.y + rec1.h >= rec2.y &&
		rec2.x + rec2.w >= rec1.x &&
		rec2.y + rec2.h >= rec1.y
		) 
	{

		return true;
	}

	return false;

}