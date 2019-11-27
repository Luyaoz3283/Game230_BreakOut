#include "bat.h"
#include <iostream>



//constructor
bat::bat() {
	boxType = batType;
}
bat::bat(int inputIndex, float inputSpeed, Vector2f inputIniLoca, Vector2f inputDimention, Window *window)
{
	setLocation(inputIniLoca);
	setDim(inputDimention);
	index = inputIndex;
	//distance per frame
	speed = inputSpeed;
	//location = inputIniLoca;
	//thisBat.setPosition(location);
	
	direction.x = 1;
	direction.y = 0;
	boxType = batType;
	this->window = window;
}

void bat::playerInput() {
	
}

void bat::update() {
	playerInput();
}

Vector2f bat::getNewDir(Vector2f collisionPoint)
{
	float offSet = 85;
	Vector2f offSetVec;
	offSetVec.x = getLocation().x + getDim().x * 0.5;
	offSetVec.y = getLocation().y + offSet;
	Vector2f newDirection = collisionPoint - offSetVec;
	return newDirection;

}




