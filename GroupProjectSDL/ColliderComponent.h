#ifndef collidercomponent_h
#define collidercomponent_h

#include "Components.h"
#include "SDL.h"
using namespace std;

class ColliderComponent : public Component {
public:

	//I initialized all the variables for the sake of clean code
	SDL_Rect collider { 0, 0, 0 ,0 };

	//Tagging like in Unity!
	string tag = "";
	int leftEdge = 0;
	int rightEdge = 0;
	int topEdge = 0;
	int bottomEdge = 0;

	TransformComponent* transform = 0;

	

	ColliderComponent(string tag) : tag(tag){
		height = 0;
		width = 0;
	};

	ColliderComponent(string tag, int height, int width):
	tag(tag), height(height), width(width){};


	ColliderComponent(string tag, int height, int width, int xOffset,
	int yOffset):
	tag(tag), height(height), width(width), xOffset(xOffset),
	yOffset(yOffset){};

	void init() override {
		
		//Add the collider component if the entity does not have one
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0,0,0,0,0,0);
		}

		transform = &entity->getComponent<TransformComponent>();

		if (width == 0){width = transform->width;} 
		if (height == 0){height = transform->height;}

		collider.w = width;
		collider.h = height;

		//Add our collider to the array of colliders
		Game::colliders.push_back(this);
	}

	void update() override {
		collider.x = SetXPos();
		collider.y = SetYPos();
		leftEdge = SetLeftEdge();
		rightEdge = SetRightEdge();
		topEdge = SetTopEdge();
		bottomEdge = SetBottomEdge();

	}

	void draw() override {
		SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 100);
		SDL_RenderFillRect(Game::renderer, &collider);
	}

private:
	int height = 0;
	int width = 0;
	int xOffset = 0;
	int yOffset = 0;

	int SetXPos() {
		int x = (static_cast<int>(transform->position.x)
		+ (transform->width / 2.0) - (width / 2.0)) + xOffset;
		return x;
	}

	int SetYPos() {
		int y = (static_cast<int>(transform->position.y)
		+ (transform->height / 2.0) - (height / 2.0)) + yOffset;
		return y;
	}


	int SetTopEdge() {
		return collider.y;
	}

	int SetBottomEdge() {
		return collider.y + height;
	}

	int SetLeftEdge() {
		return collider.x;
	}

	int SetRightEdge() {
		return collider.x + width;
	}


	
};

#endif
