#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define ESC 27

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	gameScenes[scene]->init();
	
}

bool Game::update(int deltaTime)
{
	gameScenes[scene]->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameScenes[scene]->render();
}

void Game::keyPressed(int key)
{
	if(key == GLUT_KEY_F10) 
		bPlay = false;
	if (key == 'c' && scene == LEVEL_1) {
		if (dynamic_cast<Level*> (gameScenes[scene])->getCollisionView() == true)
			dynamic_cast<Level*> (gameScenes[scene])->enableCollisionView(false);
		else
			dynamic_cast<Level*> (gameScenes[scene])->enableCollisionView(true);
	}
	if (key == 'a' && scene == LEVEL_1)
		dynamic_cast<Level*> (gameScenes[scene])->moveCamera(gameScenes[scene]->cameraLeft - 4, gameScenes[scene]->cameraRight - 4, gameScenes[scene]->cameraBottom, gameScenes[scene]->cameraTop);
	else if (key == 'd' && scene == LEVEL_1)
		dynamic_cast<Level*> (gameScenes[scene])->moveCamera(gameScenes[scene]->cameraLeft + 4, gameScenes[scene]->cameraRight + 4, gameScenes[scene]->cameraBottom, gameScenes[scene]->cameraTop);
	
	if (key == ESC && scene == LEVEL_1) {
		dynamic_cast<Menu*> (gameScenes[MENU])->setrenderMenu();
		changeScene(MENU); 
	};
	
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{		
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::changeScene(int value) 
{
	scene = value;
	if(!gameScenes[scene]->getInit())
		gameScenes[scene]->init();
	if (scene == LEVEL_1) dynamic_cast<Level*> (gameScenes[scene])->restartLevel();
}

void Game::exitGame() {
	bPlay = false;
}