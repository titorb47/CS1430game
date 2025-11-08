#ifndef collidercomponent_h
#define collidercomponent_h

#include "Components.h"
#include "SDL.h"
using namespace std;

class ColliderComponent : public Component {
public:
	SDL_Rect collider;

	//Tagging like in Unity!
	string tag;
	int leftEdge;
	int rightEdge;
	int topEdge;
	int bottomEdge;

	TransformComponent* transform;

	

	ColliderComponent(string tag) : tag(tag), 
	height(transform->height * transform->scale),
	width(transform->width * transform->scale){};

	ColliderComponent(string tag, int height, int width):
	tag(tag), height(height), width(width){};


	ColliderComponent(string tag, int height, int width, int xOffset,
	int yOffset):
	tag(tag), height(height), width(width), xOffset(xOffset),
	yOffset(yOffset){};

	void init() override {
		
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>(0,0,0);
		}

		transform = &entity->getComponent<TransformComponent>();

		collider.w = width;
		collider.h = height;
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
	int height;
	int width;
	int xOffset;
	int yOffset;

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
