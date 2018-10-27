#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


enum  Animations { Play, Instructions, Credits, Exit };


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
	background->render(animations[currentAnim].first[texture]);
	
}


void Menu::update(int deltaTime)
{
	if (currentAnim >= 0) {
		timeAnimation += deltaTime*100;
		while (timeAnimation > animations[currentAnim].second) {
			timeAnimation -= animations[currentAnim].second;
		}
		texture = !texture;
	}
}

void Menu::init() 
{
	currentAnim = Play;
	initShaders();
	animations.resize(numberAnims);
	vector<string> files = { "Resources/Menu/backgroundMenuPlay.png", "Resources/Menu/backgroundMenuInstructions.png",
									 "Resources/Menu/backgroundMenuCredits.png", "Resources/Menu/backgroundMenuExit.png" };
	

	setBackground(files,"Resources/Menu/backgroundMenuEmpty.png");



	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
	binit = true;
}


bool Menu::setBackground(vector<string> filenames, const string &baseFile)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1280, 547.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	Texture base; 
	base.loadFromFile(baseFile, TEXTURE_PIXEL_FORMAT_RGBA);

	for (int anim = Play; anim <= Exit; anim++)
	{
		Texture tex;
		tex.loadFromFile(filenames[anim], TEXTURE_PIXEL_FORMAT_RGBA);
		vector<Texture> vaux = { tex, base };
		animations[anim] = make_pair(vaux, 1000000.f);
	}
	
	return true;
}
