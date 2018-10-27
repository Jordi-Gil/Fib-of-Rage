#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"


#define WIDTH_IMAGE 1280
#define HEIGHT_IMAGE 547

#define SPEED 3

#define KEY_W 119
#define KEY_S 115

enum  Animations { PLAY, INSTR, CREDITS, EXIT };


Menu::Menu(int left, int right, int bottom, int top)
{
	cameraLeft = left;
	cameraRight = right;
	cameraBottom = bottom;
	cameraTop = top;
}

Menu::Menu() 
{

}

Menu::~Menu()
{
}

void Menu::render()
{
	Scene::render();
	background->render();
}


void Menu::update(int deltaTime)
{
	background->update(deltaTime);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (!finishOneTime) finishOneTime = background->getAnimationFinished();
	if (antDeltaTime <= currentTime && finishOneTime) {
		antDeltaTime = currentTime;
		int currentAnimation;
		if (Game::instance().getKey(KEY_W))
		{
			finishOneTime = false;
			currentAnimation = background->animation();
			if (currentAnimation == EXIT) { 
				background->changeAnimation(CREDITS); 
			}
			else if (currentAnimation == CREDITS) {
				background->changeAnimation(INSTR); 
			}
			else if (currentAnimation == INSTR) { 
				background->changeAnimation(PLAY); 
			}
		}
		else if (Game::instance().getKey(KEY_S))
		{
			finishOneTime = false;
			currentAnimation = background->animation();
			if (currentAnimation == PLAY) { 
				background->changeAnimation(INSTR); 
			}
			else if (currentAnimation == INSTR) { 
				background->changeAnimation(CREDITS); 
			}
			else if (currentAnimation == CREDITS) { 
				background->changeAnimation(EXIT); 
			}
		}
	}
}

void Menu::init() 
{
	initShaders();
	setBackground("Resources/Menu/backgroundMenu.png");
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
	binit = true;
}


bool Menu::setBackground(const string &filename)
{
	texture.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	
	background = Sprite::createSprite(glm::ivec2(WIDTH_IMAGE, HEIGHT_IMAGE), glm::vec2(0.2f, 1.f), &texture, &texProgram);
	background->setNumberAnimations(4);

	background->setAnimationSpeed(PLAY, SPEED);
	background->addKeyframe(PLAY, glm::vec2(0.f, 1.f));
	background->addKeyframe(PLAY, glm::vec2(0.2f, 1.0f));

	background->setAnimationSpeed(INSTR, SPEED);
	background->addKeyframe(INSTR, glm::vec2(0.f, 1.f));
	background->addKeyframe(INSTR, glm::vec2(0.4f, 1.0f));

	background->setAnimationSpeed(CREDITS, SPEED);
	background->addKeyframe(CREDITS, glm::vec2(0.f, 1.f));
	background->addKeyframe(CREDITS, glm::vec2(0.6f, 1.0f));

	background->setAnimationSpeed(EXIT, SPEED);
	background->addKeyframe(EXIT, glm::vec2(0.f, 1.f));
	background->addKeyframe(EXIT, glm::vec2(0.8f, 1.0f));

	background->changeAnimation(PLAY);

	background->setPosition(glm::vec2(0, 0));
	return true;
}
