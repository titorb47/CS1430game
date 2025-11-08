#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "Components.h"
#include "Vector2D.h"


//See ECS.h
struct TransformComponent : public Component {

	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	double speed;

	TransformComponent(double speed) {
		position.Zero();
		this->speed = speed;
	}


	TransformComponent(double x, double y, double speed) {
		position.x = x;
		position.y = y;
		this->speed = speed;
	}

	TransformComponent(double x, double y, double speed, int h, int w, int s){
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = s;
		this->speed = speed;
	}
	
	void init() override {
		velocity.Zero();
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};

#endif