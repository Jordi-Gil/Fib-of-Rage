#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"


#define WIDTH_IMAGE 1280
#define HEIGHT_IMAGE 547

#define SPEED 5

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_ENTER 13
#define ESC 27

enum  Animations { PLAY, INSTR, CREDITS, EXIT };
/*
enum  PLAYERS { RYU, HONDA, BISON };
*/

Menu::Menu(int left, int right, int bottom, int top)
{
	cameraLeft = left;
	cameraRight = right;
	cameraBottom = bottom;
	cameraTop = top;
	index = RYU;
	credits = NULL;
	controls = NULL;
	playersView = NULL;
	menu = NULL;
}

Menu::Menu() 
{
	index = RYU;
	credits = NULL;
	controls = NULL;
	playersView = NULL;
	menu = NULL;
}

Menu::~Menu()
{
	if (credits != NULL)
		delete credits;
	if (controls != NULL)
		delete controls;
	if (playersView != NULL)
		delete playersView;
	if (menu != NULL)
		delete menu;

}

void Menu::render()
{
	Scene::render();
	if(renderMenu) menu->render();
	else if (renderCont) controls->render(contrTex);
	else if (renderCred) credits->render(credTex);
	else if (renderPlayers) playersView->render();
}


void Menu::update(int deltaTime)
{
	if(renderMenu) menu->update(deltaTime);
	else playersView->update(deltaTime);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	
	if (!finishOneTime) finishOneTime = ( renderMenu ? menu->getAnimationFinished() : playersView->getAnimationFinished());
	
	if (antDeltaTime <= currentTime && finishOneTime) {
		antDeltaTime = currentTime;
		int currentAnimation;
		if (Game::instance().getKey(KEY_W) || Game::instance().getSpecialKey(GLUT_KEY_UP))
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
		else if (Game::instance().getKey(KEY_S) || Game::instance().getSpecialKey(GLUT_KEY_DOWN))
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
		else if (Game::instance().getKey(KEY_D) || Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			finishOneTime = false;
			currentAnimation = playersView->animation();
			if (currentAnimation == RYU) {
				index = HONDA;
				playersView->changeAnimation(HONDA);
			}
			else if (currentAnimation == HONDA) {
				index = BISON;
				playersView->changeAnimation(BISON);
			}
			else if (currentAnimation == BISON) {
				index = RYU;
				playersView->changeAnimation(RYU);
			}
		}
		else if (Game::instance().getKey(KEY_A) || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			finishOneTime = false;
			currentAnimation = playersView->animation();
			if (currentAnimation == RYU) {
				index = BISON;
				playersView->changeAnimation(BISON);
			}
			else if (currentAnimation == BISON) {
				index = HONDA;
				playersView->changeAnimation(HONDA);
			}
			else if (currentAnimation == HONDA) {
				index = RYU;
				playersView->changeAnimation(RYU);
			}
		}
		else if (Game::instance().getKey(KEY_ENTER)) {
			if(renderMenu){
				currentAnimation = menu->animation();
				if (currentAnimation == PLAY) {
					renderMenu = false;
					renderCont = false;
					renderCred = false;
					renderPlayers = true;
					index = RYU;
					selected_player = false;
					Game::instance().keyReleased(KEY_ENTER);
				}
				else if (currentAnimation == INSTR) {
					renderMenu = false;
					renderCont = true;
					renderCred = false;
					renderPlayers = false;
				}
				else if (currentAnimation == CREDITS) {
					renderMenu = false;
					renderCont = false;
					renderCred = true;
					renderPlayers = false;
				}
				
				else Game::instance().exitGame();
			}
			else if (renderPlayers) {
				currentAnimation = playersView->animation();
				if (currentAnimation == RYU) {
					Game::instance().setPlayer(RYU);
					Game::instance().changeScene(LEVEL_1,RYU);
				}
				else if (currentAnimation == HONDA) {
					Game::instance().setPlayer(RYU);
					Game::instance().changeScene(LEVEL_1, HONDA);
				}
				if (currentAnimation == BISON) {
					Game::instance().setPlayer(BISON);
					Game::instance().changeScene(LEVEL_1, BISON);
				}
			}
		}
		else if (Game::instance().getKey(ESC)) {
			currentAnimation = menu->animation();
			if (renderCont || renderCred) {
				renderMenu = true;
				renderCont = false;
				renderCred = false;
				renderPlayers = false;
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
	setPlayers("Resources/Menu/SelectCharacter/select-character.png");
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
	binit = true;
}


void Menu::setMenu(const string &filename)
{
	menuTex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	
	menu = Sprite::createSprite(glm::ivec2(WIDTH_IMAGE, HEIGHT_IMAGE), glm::vec2(0.2f, 1.f), &menuTex, &texProgram);
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

void Menu::setPlayers(const string &filename)
{
	playersTex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

	playersView = Sprite::createSprite(glm::ivec2(WIDTH_IMAGE, HEIGHT_IMAGE), glm::vec2(1 / 3.f, 1.f), &playersTex, &texProgram);
	playersView->setNumberAnimations(3);

	playersView->setAnimationSpeed(RYU, SPEED);
	playersView->addKeyframe(RYU, glm::vec2(0.f, 1.f));
	

	playersView->setAnimationSpeed(HONDA, SPEED);
	playersView->addKeyframe(HONDA, glm::vec2(1 / 3.f, 1.f));
	

	playersView->setAnimationSpeed(BISON, SPEED);
	playersView->addKeyframe(BISON, glm::vec2(2 / 3.f, 1.f));
	

	playersView->changeAnimation(RYU);

	playersView->setPosition(glm::vec2(0, 0));
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
	renderCont = false;
	renderCred = false;
	renderPlayers = false;
}