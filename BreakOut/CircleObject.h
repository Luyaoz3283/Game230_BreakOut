#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class CircleObject
{
protected:
	
	float radius;
	Vector2f direction;
public:
	//???change to protected
	CircleShape thisBall;
	void setLocation(Vector2f inputLocation);
	Vector2f getLocation();
	float getRadius();
	//???
	void wallCollision(Vector2f screenDim);
};

