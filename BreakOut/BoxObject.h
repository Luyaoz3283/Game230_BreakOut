#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class BoxObject
{
private:
	
	Vector2f dimension;

	
protected:
	
	

public:
	int boxIndex = -1;
	RectangleShape thisRect;
	BoxObject();
	enum boxTypeList { batType, blockType };
	boxTypeList boxType;

	Vector2f getLocation();
	void setLocation(Vector2f inputLocation);
	Vector2f getDim();
	void setDim(Vector2f inputDim);
	virtual void update();
	void draw(RenderWindow* theWindow);
	void setColor(Color targetColor);
};

