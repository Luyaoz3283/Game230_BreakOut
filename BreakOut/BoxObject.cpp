#include "BoxObject.h"






BoxObject::BoxObject()
{
}

Vector2f BoxObject::getLocation() {
	return thisRect.getPosition();
}

void BoxObject::setLocation(Vector2f inputLocation)
{
	thisRect.setPosition(inputLocation);
}

Vector2f BoxObject::getDim() {
	return thisRect.getSize();
}

void BoxObject::setDim(Vector2f inputDim)
{
	thisRect.setSize(inputDim);
}

void BoxObject::draw(RenderWindow* theWindow) {
	theWindow->draw(thisRect);
}

void BoxObject::setColor(Color targetColor)
{
	thisRect.setFillColor(targetColor);
}

void BoxObject::update() {

}




