#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


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
	background->render(tex);
}


void Menu::init() 
{
	initShaders();
	setBackground("Resources/Menu/Background.png");
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
	binit = true;
}


bool Menu::setBackground(const string &filename)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1600.f, 1200.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	tex.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

	return true;
}
