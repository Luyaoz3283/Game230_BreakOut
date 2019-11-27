#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "BoxObject.h"

using namespace sf;


class bat:public BoxObject
{
private:
	void playerInput(); 
	
public:
	
	
	Window* window;
	int score = 0;
	int index;
	float speed;
	Vector2f direction;
	//Vector2f dimension;
	bat();
	bat(int inputIndex, float inputSpeed, Vector2f inputIniLoca, Vector2f inputDimention, Window *window);
	//Vector2f getLocation();
	//void setLocation(Vector2f inputLocation);
	//Vector2f getDim();
	//void update();
	void update();
	Vector2f getNewDir(Vector2f collisionPoint);

};

