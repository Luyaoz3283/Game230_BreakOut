#include <iostream>
#include "block.h"

using namespace std;
using namespace sf;

block::block() {
	boxType = blockType;
}
block::block(Vector2f inputDimension, Vector2f inputLocation)
{
	boxType = blockType;
	setDim(inputDimension);
	setLocation(inputLocation);
}

void block::update()
{
}

void block::setTexture(Texture *inputTex)
{
	thisRect.setTexture(inputTex);
}




