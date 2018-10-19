#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene(int left, int right, int bottom, int top)
{
	fullMap.scenario = NULL;
	fullMap.collision = NULL;
	player = NULL;
	cameraLeft = left;
	cameraRight = right;
	cameraBottom = bottom;
	cameraTop = top;
}
Scene::Scene()
{
	fullMap.scenario = NULL;
	fullMap.collision = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(fullMap.scenario != NULL)
		delete fullMap.scenario;
	if (fullMap.collision != NULL)
		delete fullMap.collision;
	if(player != NULL)
		delete player;
}

void Scene::init()
{
	initShaders();
	fullMap.scenario = TileMap::createTileMap("levels/level01_object.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//fullMap.collision = TileMap::createTileMap("levels/level01_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	setBackground();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * fullMap.scenario->getTileSize(), INIT_PLAYER_Y_TILES * fullMap.scenario->getTileSize()));
	player->setTileMap(fullMap.scenario);
	projection = glm::ortho(float(cameraLeft), float(cameraRight), float(cameraBottom), float(cameraTop));
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	fullMap.scenario->render();
	if(showCollisions) fullMap.collision->render();
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
void Scene::enableCollisionView(bool state) {
	showCollisions = state;
}
bool Scene::getCollisionView() {
	return showCollisions;
}
void Scene::moveCamera(int left, int right, int bottom, int top) {
	if (left >= 0 && right <= fullMap.scenario->getTileSize()) {
		this->cameraLeft = left;
		this->cameraRight = right;
		this->cameraBottom = bottom;
		this->cameraTop = top;
		projection = glm::ortho(float(left), float(right), float(bottom), float(top));
	}
}

void Scene::setBackground()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.5f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(2.f, 2.f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/rocks.jpg", TEXTURE_PIXEL_FORMAT_RGB);
}


