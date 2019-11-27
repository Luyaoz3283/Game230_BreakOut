#include "CircleObject.h"

void CircleObject::setLocation(Vector2f inputLocation)
{
	thisBall.setPosition(inputLocation);
}

Vector2f CircleObject::getLocation() {
	return thisBall.getPosition();
}

float CircleObject::getRadius() {
	return thisBall.getRadius();
}

void CircleObject::wallCollision(Vector2f screenDim) {
	if (thisBall.getPosition().x + 2 * radius > 0
		&& thisBall.getPosition().x < screenDim.x
		&& (thisBall.getPosition().y <= 0
			|| thisBall.getPosition().y + 2 * radius >= screenDim.y)) {
		direction.y = direction.y * (-1);
	};
}