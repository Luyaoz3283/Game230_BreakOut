#pragma once
#include <iostream>;
#include <SFML/Graphics.hpp>
#include "CircleObject.h"

using namespace sf;


class ball:public CircleObject
{
private:
	float speed;
	
	Vector2f iniLoc;
	
	void autoMove();
	void playerInput();
	
	

public:
	enum gameStateList { aiming, launched, gameOver };
	static gameStateList gameState;
	
	ball(float inputSpeed, float inputRadius, Vector2f inputIniLocation, Vector2f inputIniDirection);
	
	
	
	void update();
	

	void symmetricBounce(float xDir, float yDir);
	void setDir(Vector2f newUnitDir);
	Vector2f getUnitVec(Vector2f rawVector);
	void initialize(float inputSpeed, float inputRadius, Vector2f inputIniLocation, Vector2f inputIniDirection);
	void setSpeed(float input);
	void speedUp();
};

