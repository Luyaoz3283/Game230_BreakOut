#include <iostream>;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "ball.h"
#include "bat.h"
#include "block.h"

using namespace std;
using namespace sf;



//set frame rate
float FPS = 100;
//set screen dimension
Vector2f screenDim(1000, 800);
//set bat parameters
float batSpeed = 7;
Vector2f batDimension(100, 30);
Color bat1Color = Color::Blue;
float batBottomOffSet = 50;
Vector2f bat1IniLoc(screenDim.x* 0.5, screenDim.y - batBottomOffSet);
Vector2f batLastLocation;
//set ball parameters
float ballRadius = 20.0f;
float ballSpeed = 3;
Vector2f iniDirection(1,1);
Vector2f ballIniLocation = Vector2f(bat1IniLoc.x + batDimension.x * 0.5 - ballRadius, bat1IniLoc.y + ballRadius*2);
//Vector2f ballIniLocation = Vector2f(bat1IniLoc.x + batDimension.x * 0.5 - ballRadius, bat1IniLoc.y - ballRadius*2);
Color ballColor = Color::Yellow;
//set blocks parameters
Vector2f blockDimension(120, 30);
Vector2f blockIniLoc(50, 50);
float blockXInterval = 1, blockYInterval = 1;
const int blockRows = 1, blockCols = 1;
BoxObject* preBlock;


//set sound
SoundBuffer hitPaddle;
SoundBuffer destroyBrick;
SoundBuffer bounceWall;
SoundBuffer lose;
SoundBuffer win;
Sound sound;
Sound sound2;
//set display
Text textLife;
Text textScore;
Text textLevel;
Font font;
Texture bgTexture;
Texture brickImage;
Sprite backGroundSprite;
//game state
bool passedBound = false;
bool hasGameEnded = false;
bool inMenuPage = true;

//set array
vector<BoxObject*> boxList;
ball *ball1;
bat *bat1;
block *block1;
block blockMatrix[blockRows][blockCols];

//enum
enum inputOptionList { keyboard, mouse };
inputOptionList inputOption = keyboard;

//global variables
RenderWindow window(sf::VideoMode(screenDim.x, screenDim.y), "SFML works!");
int maxLifeRemaining = 1;
int lifeRemaining = maxLifeRemaining;
int score;
ball::gameStateList ball::gameState;
int level = 1;


void scoreDisplay(int inputScore) {
	score = inputScore;
	string displayScore = "Score:" + std::to_string(score);
	textScore.setString(displayScore);
}
void lifeDisplay(int inputLife) {
	lifeRemaining = inputLife;
	string displayLife = "life remaining:" + std::to_string(lifeRemaining);
	textLife.setString(displayLife);
}
void levelDisplay(int inputLevel) {
	level = inputLevel;
	string displayLevel = "Level:" + std::to_string(level);
	textLevel.setString(displayLevel);
}

Vector2f getUnitVec(Vector2f vector) {
	float length = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	Vector2f unitVector = vector / length;
	return unitVector;
}

void generateBlock() {
	for (int i = 0; i < blockRows; ++i) {
		for (int j = 0; j < blockCols; ++j) {
			blockMatrix[i][j] = block(blockDimension, blockIniLoc + Vector2f(blockXInterval * j + blockDimension.x * j, blockYInterval * i + blockDimension.y * i));
			blockMatrix[i][j].index1 = i;
			blockMatrix[i][j].index2 = j;
			if (i == 4 && j == 4) {
				blockMatrix[4][4].setColor(Color::Yellow);
			}
			blockMatrix[i][j].setTexture(&brickImage);
		}
	}
	for (int i = 0; i < blockRows; i++) {
		for (int j = 0; j < blockCols; j++) {
			boxList.push_back(&blockMatrix[i][j]);

		}
	}
}

void startRound() {
	ball1->initialize(ballSpeed, ballRadius, ballIniLocation, iniDirection);
}

void nextLevel() {
	sound2.setBuffer(win);
	sound2.play();
	generateBlock();
	startRound();
	levelDisplay(++level);
	ball1->speedUp();
}

bool checkBallRectCollision(ball *targetBall, BoxObject *targetBox) {
	Vector2f ballOrigin = targetBall->getLocation();
	Vector2f rectOrigin = targetBox->getLocation();
	float ballRadius = targetBall->getRadius();
	Vector2f rectDim = targetBox->getDim();
	float dx = 0, dy = 0;
	float horiEdge = ballOrigin.y + ballRadius, vertiEdge = ballOrigin.x + ballRadius;
	float xDir = 0, yDir = 0;
	if (ballOrigin.y + 2* ballRadius <= rectOrigin.y) {
		horiEdge = rectOrigin.y;
		yDir = -1;
	}
	else if (ballOrigin.y>= rectOrigin.y + rectDim.y) {
		horiEdge = rectOrigin.y + rectDim.y;
		yDir = 1;
	}
	if (ballOrigin.x + 2*ballRadius <= rectOrigin.x) {
		vertiEdge = rectOrigin.x;
		xDir = -1;
	}
	else if (ballOrigin.x >= rectOrigin.x + rectDim.x) {
		vertiEdge = rectOrigin.x + rectDim.x;
		xDir = 1;
	}
	dx = ballOrigin.x + ballRadius - vertiEdge;
	dy = ballOrigin.y + ballRadius - horiEdge;
	float distance = sqrt(pow(dx,2) + pow(dy,2));
	if (distance <= ballRadius + ballSpeed) {
		//cout << "test";
		if (targetBox->boxType == BoxObject::boxTypeList::batType) {
			bat* theBat = dynamic_cast<bat*>(targetBox);
			Vector2f newDir = theBat->getNewDir(Vector2f(ballOrigin.x + ballRadius, ballOrigin.y + 2 * ballRadius));
			targetBall->setDir(newDir);
			if (ball::gameState != ball::gameStateList::aiming) {
				sound.setBuffer(hitPaddle);
				sound.play();
			}
		}
		else if (targetBox->boxType == BoxObject::boxTypeList::blockType) {
			targetBall->symmetricBounce(xDir, yDir);
			auto it = find(boxList.begin(), boxList.end(), targetBox);
			if (it != boxList.end()) {
				boxList.erase(it);
			}
			scoreDisplay(score + 10);
			//play sound
			sound.setBuffer(destroyBrick);
			sound.play();
			//?????haven't deleted the block object
			if (boxList.size() - 1 <= 0) {
				nextLevel();
			}
		}
		return true;
	}
	return false;
}

bool checkWallCollision(ball* targetBall) {
	Vector2f ballOrigin = targetBall->getLocation();
	float ballRadius = targetBall->getRadius();
	bool collided = false;
	float xDir = 0, yDir = 0;
	float vertiEdge = 0, horiEdge = 0;
	if (ballOrigin.x + ballRadius <= screenDim.x * 0.5f) {
		vertiEdge = 0;
	}
	else {
		vertiEdge = screenDim.x;
	}
	horiEdge = 0;

	float dx = ballOrigin.x + ballRadius - vertiEdge;
	float dy = ballOrigin.y + ballRadius - horiEdge;
	if (pow(dx,2) <= pow(ballRadius, 2)) {
		xDir = dx;
		collided = true;
	}
	if (pow(dy, 2) <= pow(ballRadius, 2)) {
		yDir = dy;
		collided = true;
	}
	if (collided) {
		targetBall->symmetricBounce(xDir, yDir);
		sound.setBuffer(bounceWall);
		sound.play();
		return true;
	}

}
void gameEnded() {
	ball::gameState = ball::gameStateList::gameOver;
	sound.setBuffer(lose);
	sound.play();
	window.clear();
	string display = "GAME OVER \n Score:" + to_string(score);
	textScore.setString(display);
	textScore.setPosition(Vector2f(screenDim.x * 0.1, 200));
	textScore.setCharacterSize(50);
	window.draw(backGroundSprite);
	window.draw(textScore);
	window.display();
}

bool checkGroundCollision() {
	if (ball1->getLocation().y >= screenDim.y) {
		startRound();
		
		lifeDisplay(lifeRemaining - 1);
		if (lifeRemaining <= 0) {
			gameEnded();
		}
		
		//setScoreLifeDisplayText();
		return true;
	}
	return false;
}

bool checkIfPassBoundary() {
	return false;
}
//bat and ball player input
void playerInput() {
	Vector2f desiredLoc = ballIniLocation;
	//decide keyboard or mouse
	Vector2f mousePos1(Mouse::getPosition(window));
	if (mousePos1.x != batLastLocation.x) {
		inputOption = inputOptionList::mouse;
	}
	else {
		inputOption = inputOptionList::keyboard;
	}
	batLastLocation = mousePos1;
	//get player input
	if (inputOption == inputOptionList::keyboard) {
		if (Keyboard::isKeyPressed(sf::Keyboard::A)) {
			bat1->direction.x = -1;
			desiredLoc = bat1->getLocation() + bat1->speed * bat1->direction;
			bat1->setLocation(desiredLoc);
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			bat1->direction.x = 1;
			desiredLoc = bat1->getLocation() + bat1->speed * bat1->direction;
			bat1->setLocation(desiredLoc);
		}
	}
	else {
		Vector2f mousePos(Mouse::getPosition(window));
		desiredLoc = Vector2f(mousePos.x, bat1->getLocation().y);
		bat1->setLocation(desiredLoc);
	}
	if (ball::gameState == ball1->gameStateList::aiming) {
		Vector2f batLocation = bat1->getLocation();
		ball1->setLocation(Vector2f(batLocation.x + batDimension.x * 0.5, batLocation.y - 2 * ballRadius));
	}
}

void mainUpdate() {
	if (ball::gameState != ball::gameStateList::gameOver) {
		playerInput();
		bat1->update();
		ball1->update();
		//check collision
		checkWallCollision(ball1);
		for (int i = 0; i < boxList.size(); i++) {
			checkBallRectCollision(ball1, boxList[i]);
		}
		checkGroundCollision();
	}
}

void loadGame() {
	//load audio
	if (!hitPaddle.loadFromFile("hitPaddle.wav")) {
		cout << "sound not loaded";
	}
	if (!destroyBrick.loadFromFile("destroyBrick.wav")) {
		cout << "sound not loaded";
	}
	if (!bounceWall.loadFromFile("bounceWall.wav")) {
		cout << "sound not loaded";
	}
	if (!lose.loadFromFile("lose.wav")) {
		cout << "sound not loaded";
	}
	if (!win.loadFromFile("win.wav")) {
		cout << "sound not loaded";
	}
	//load texture
	if (!bgTexture.loadFromFile("bgImage.png"))
	{
		cout << "texture not loaded";
	}
	if (!brickImage.loadFromFile("test.png"))
	{
		cout << "texture not loaded";
	}
	backGroundSprite.setTexture(bgTexture);
	if (!font.loadFromFile("stocky.ttf"))
	{
		std::cout << "Error loading font\n";
	}
	textLevel.setFont(font);
	textLevel.setPosition(Vector2f(screenDim.x * 0.1, 700));
	textLevel.setCharacterSize(50);
	textScore.setFont(font);
	textScore.setPosition(Vector2f(screenDim.x * 0.1, 600));
	textScore.setCharacterSize(50);
	textLife.setFont(font);
	textLife.setPosition(Vector2f(screenDim.x * 0.1, 500));
	textLife.setCharacterSize(50);
	
	scoreDisplay(0);
	lifeDisplay(maxLifeRemaining);
	levelDisplay(1);
	//initialize shape parameters and locations
	ball1 = new ball(ballSpeed, ballRadius, ballIniLocation, iniDirection);
	bat1 = new bat(1, batSpeed, bat1IniLoc, batDimension, &window);
	boxList.push_back(bat1);
	
	
	
}

void drawGraph() {
	if (ball::gameState != ball::gameStateList::gameOver) {
		window.clear();
		window.draw(backGroundSprite);
		window.draw(ball1->thisBall);
		for (int i = 0; i < boxList.size(); i++) {
			boxList[i]->draw(&window);
		}
		window.draw(textLife);
		window.draw(textScore);
		window.draw(textLevel);
		//test:
		window.display();
	}
}

void startGame() {
	generateBlock();
	startRound();
	lifeDisplay(maxLifeRemaining);
	scoreDisplay(0);
	level = 0;
	//ball::gameState = ball::gameStateList::aiming;
}



int main()
{
	loadGame();
	startGame();
	Clock clock;
	Time elapsed1 = clock.getElapsedTime();
	sf::Time frame = sf::seconds(1/FPS);
	//close window
	while (window.isOpen())
	{
		//open and close the window
		sf::Event event;	
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		Time elapsed2 = clock.getElapsedTime();
		if (elapsed2 - elapsed1 > frame) {
			elapsed1 = clock.getElapsedTime();
			mainUpdate();
			drawGraph();
			//check restart
			if (ball::gameState == ball::gameStateList::gameOver && Keyboard::isKeyPressed(Keyboard::R)) {
				startGame();
			}
		}
	}

	return 0;
}
