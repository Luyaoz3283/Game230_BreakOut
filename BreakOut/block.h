#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "BoxObject.h"

using namespace sf;



class block:public BoxObject
{
private:
	//RectangleShape thisBlock;
	Color color;

public:
	int index1;
	int index2;
	Window *window;
	block();
	block(Vector2f inputDimension, Vector2f inputLocation);
	void update();
	void setTexture(Texture *inputTex);


};

