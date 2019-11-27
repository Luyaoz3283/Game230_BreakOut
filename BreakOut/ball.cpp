#include "ball.h"
using namespace sf;



ball::ball(float inputSpeed, float inputRadius, Vector2f inputIniLocation, Vector2f inputIniDirection) {
	
	initialize(inputSpeed, inputRadius, inputIniLocation, inputIniDirection);
}
void ball::autoMove() {
	if (gameState == launched) {
		Vector2f location = getLocation() + speed * direction;
		setLocation(location);
	}
	
	
}

void ball::playerInput() {
	if (Keyboard::isKeyPressed(Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		gameState = launched;
	}
}
void ball::initialize(float inputSpeed, float inputRadius, Vector2f inputIniLocation, Vector2f inputIniDirection)
{
	speed = inputSpeed;
	radius = inputRadius;
	setLocation(inputIniLocation);
	direction = getUnitVec(inputIniDirection);
	thisBall.setRadius(inputRadius);
	//game state
	gameState = gameStateList::aiming;
}
void ball::setSpeed(float input)
{
	speed = input;
}
void ball::speedUp()
{
	speed += 0.7f*speed;
}



void ball::update() {
	autoMove();
	playerInput();
}


void ball::symmetricBounce(float xDir, float yDir) {
	if ((xDir < 0 && direction.x > 0) || (xDir > 0 && direction.x < 0)) {
		direction.x = direction.x * (-1);
	}
	if ((yDir < 0 && direction.y > 0) || (yDir > 0 && direction.y < 0)) {
		direction.y = direction.y * (-1);
	}
}

void ball::setDir(Vector2f newDir)
{
	
	direction = getUnitVec(newDir);
}


Vector2f ball::getUnitVec(Vector2f rawVector)
{
	float length = sqrt(pow(rawVector.x, 2) + pow(rawVector.y, 2));
	Vector2f unitVec = rawVector / length;
	return unitVec;
}




