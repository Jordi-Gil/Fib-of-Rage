#include "Level.h"
												
Level::Level(int left, int right, int bottom, int top) : Scene(left, right, bottom, top)
{
	
}

Level::Level()
{
}

Level::~Level()
{

}

void Level::init()
{
	
}

void Level::render()
{
	Scene::render();
}

void Level::update(int deltaTime) {
	Scene::update(deltaTime);
}

void Level::enableCollisionView(bool state) {
	showCollisions = state;
}

bool Level::getCollisionView() {
	return showCollisions;
}


