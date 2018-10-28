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
#define KEY_ENTER 13
#define ESC 27

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
	if(renderMenu) menu->render();
	else if (renderCont) controls->render(contrTex);
	else if (renderCred) credits->render(credTex);
}


void Menu::update(int deltaTime)
{
	menu->update(deltaTime);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (!finishOneTime) finishOneTime = menu->getAnimationFinished();
	if (antDeltaTime <= currentTime && finishOneTime) {
		antDeltaTime = currentTime;
		int currentAnimation;
		if (Game::instance().getKey(KEY_W))
		{
			finishOneTime = false;
			currentAnimation = menu->animation();
			if (currentAnimation == EXIT) { 
				menu->changeAnimation(CREDITS);
			}
			else if (currentAnimation == CREDITS) {
				menu->changeAnimation(INSTR);
			}
			else if (currentAnimation == INSTR) { 
				menu->changeAnimation(PLAY);
			}
		}
		else if (Game::instance().getKey(KEY_S))
		{
			finishOneTime = false;
			currentAnimation = menu->animation();
			if (currentAnimation == PLAY) { 
				menu->changeAnimation(INSTR);
			}
			else if (currentAnimation == INSTR) { 
				menu->changeAnimation(CREDITS);
			}
			else if (currentAnimation == CREDITS) { 
				menu->changeAnimation(EXIT);
			}
		}
		else if (Game::instance().getKey(KEY_ENTER)) {
			currentAnimation = menu->animation();
			if (currentAnimation == PLAY) {
				renderMenu = false;
				renderCont = false;
				renderCred = false;
				Game::instance().changeScene(LEVEL_1);
			}
			else if (currentAnimation == INSTR) {
				renderMenu = false;
				renderCont = true;
				renderCred = false;
			}
			else if (currentAnimation == CREDITS) {
				renderMenu = false;
				renderCont = false;
				renderCred = true;
			}
		}
		else if (Game::instance().getKey(ESC)) {
			currentAnimation = menu->animation();
			if (renderCont || renderCred) {
				renderMenu = true;
				renderCont = false;
				renderCred = false;
			}
		}
	}
}

void Menu::init() 
{
	initShaders();
	setMenu("Resources/Menu/backgroundMenu.png");
	setCtrl("Resources/Menu/Instructions/keyboard.png");
	setCred("Resources/Menu/Credits/credits.png");
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
	binit = true;
}


void Menu::setMenu(const string &filename)
{
	texture.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	
	menu = Sprite::createSprite(glm::ivec2(WIDTH_IMAGE, HEIGHT_IMAGE), glm::vec2(0.2f, 1.f), &texture, &texProgram);
	menu->setNumberAnimations(4);

	menu->setAnimationSpeed(PLAY, SPEED);
	menu->addKeyframe(PLAY, glm::vec2(0.f, 1.f));
	menu->addKeyframe(PLAY, glm::vec2(0.2f, 1.0f));

	menu->setAnimationSpeed(INSTR, SPEED);
	menu->addKeyframe(INSTR, glm::vec2(0.f, 1.f));
	menu->addKeyframe(INSTR, glm::vec2(0.4f, 1.0f));

	menu->setAnimationSpeed(CREDITS, SPEED);
	menu->addKeyframe(CREDITS, glm::vec2(0.f, 1.f));
	menu->addKeyframe(CREDITS, glm::vec2(0.6f, 1.0f));

	menu->setAnimationSpeed(EXIT, SPEED);
	menu->addKeyframe(EXIT, glm::vec2(0.f, 1.f));
	menu->addKeyframe(EXIT, glm::vec2(0.8f, 1.0f));

	menu->changeAnimation(PLAY);

	menu->setPosition(glm::vec2(0, 0));
	
}

void Menu::setCtrl(const string &filename)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1280.f, 547.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	controls = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	contrTex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
}

void Menu::setCred(const string &filename)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1280.f, 547.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	credits = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	credTex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
}

void Menu::setrenderMenu()
{
	renderMenu = true;
}